
template <typename T>
class SmallCandidateDecode {
public:
    __aicore__ inline void Init(GM_ADDR raw, GM_ADDR prior, GM_ADDR out,
                               const FireCandidateDecodeFusionTilingData &d)
    {
        n_ = d.candidates; total_ = d.batch * n_; cores_ = d.cores;
        raw_.SetGlobalBuffer((__gm__ T *)raw);
        prior_.SetGlobalBuffer((__gm__ float *)prior);
        out_.SetGlobalBuffer((__gm__ T *)out);
        pipe_.InitBuffer(inRaw_, 1, 32);
        pipe_.InitBuffer(inPrior_, 1, 32);
        pipe_.InitBuffer(outQueue_, 1, 32);
        pipe_.InitBuffer(sigBuf_, 32);
        pipe_.InitBuffer(resultBuf_, 32);
        pipe_.InitBuffer(oneBuf_, 32);
        Duplicate(oneBuf_.Get<float>(), 1.0f, 8);
    }
    __aicore__ inline void Process()
    {
        for (uint32_t i = GetBlockIdx(); i < total_; i += cores_) {
            auto raw = inRaw_.AllocTensor<T>();
            auto prior = inPrior_.AllocTensor<float>();
            DataCopyPad(raw, raw_[i * 6],
                DataCopyExtParams{1, 6 * uint32_t(sizeof(T)), 0, 0, 0},
                DataCopyPadExtParams<T>{false, 0, 0, 0});
            DataCopyPad(prior, prior_[(i % n_) * 5],
                DataCopyExtParams{1, 20, 0, 0, 0},
                DataCopyPadExtParams<float>{false, 0, 0, 0});
            inRaw_.EnQue(raw); inPrior_.EnQue(prior);
            raw = inRaw_.DeQue<T>(); prior = inPrior_.DeQue<float>();
            auto sig = sigBuf_.Get<float>();
            if constexpr (sizeof(T) == 2)
                Cast(sig, raw, RoundMode::CAST_NONE, 6);
            else
                DataCopy(sig, raw, 8);
            PipeBarrier<PIPE_V>();
            Muls(sig, sig, -1.0f, 6);
            PipeBarrier<PIPE_V>();
            Exp(sig, sig, 6);
            PipeBarrier<PIPE_V>();
            Adds(sig, sig, 1.0f, 6);
            PipeBarrier<PIPE_V>();
            Div(sig, oneBuf_.Get<float>(), sig, 6);
            auto vs = pipe_.FetchEventID(HardEvent::V_S);
            SetFlag<HardEvent::V_S>(vs);
            WaitFlag<HardEvent::V_S>(vs);
            auto result = resultBuf_.Get<float>();
            const float stride = prior.GetValue(2);
            float x = 2.0f * sig.GetValue(0);
            x = x - 0.5f;
            x = x + prior.GetValue(0);
            x = x * stride;
            float y = 2.0f * sig.GetValue(1);
            y = y - 0.5f;
            y = y + prior.GetValue(1);
            y = y * stride;
            float w = 2.0f * sig.GetValue(2);
            w = w * w;
            w = w * prior.GetValue(3);
            w = w * stride;
            float h = 2.0f * sig.GetValue(3);
            h = h * h;
            h = h * prior.GetValue(4);
            h = h * stride;
            const float score = sig.GetValue(4) * sig.GetValue(5);
            result.SetValue(0, x); result.SetValue(1, y);
            result.SetValue(2, w); result.SetValue(3, h);
            result.SetValue(4, score);
            auto sv = pipe_.FetchEventID(HardEvent::S_V);
            SetFlag<HardEvent::S_V>(sv);
            WaitFlag<HardEvent::S_V>(sv);
            auto dst = outQueue_.AllocTensor<T>();
            if constexpr (sizeof(T) == 2)
                Cast(dst, result, RoundMode::CAST_RINT, 5);
            else
                DataCopy(dst, result, 8);
            outQueue_.EnQue(dst); dst = outQueue_.DeQue<T>();
            DataCopyPad(out_[i * 5], dst,
                DataCopyExtParams{1, 5 * uint32_t(sizeof(T)), 0, 0, 0});
            outQueue_.FreeTensor(dst);
            inRaw_.FreeTensor(raw); inPrior_.FreeTensor(prior);
        }
    }
private:
    TPipe pipe_;
    TQue<QuePosition::VECIN, 1> inRaw_, inPrior_;
    TQue<QuePosition::VECOUT, 1> outQueue_;
    TBuf<TPosition::VECCALC> sigBuf_, resultBuf_, oneBuf_;
    GlobalTensor<T> raw_, out_;
    GlobalTensor<float> prior_;
    uint32_t n_, total_, cores_;
};
