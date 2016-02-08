#include <Pigment/RenderStateSettings.hpp>

namespace pigment
{
    using namespace stick;

    RenderStateSettings::RenderStateSettings() :
        m_renderState(0)
    {
        //set more defaults
        setColorWrite(true, true, true, true);
        setDepthWrite(false);
        setFrontFaceDirection(FaceDirection::CounterClockwise);
        setCullFace(FaceType::None);
        setMultisampling(false);
        setDepthTest(false);
        setDepthFunction(CompareFunction::Less);
        setBlending(false);
        setBlendFunction(BlendFunction::SourceAlpha, BlendFunction::InverseSourceAlpha);
    }

    RenderStateSettings & RenderStateSettings::setBlending(bool _b)
    {
        return setFlag(RF_Blending, _b);
    }

    RenderStateSettings & RenderStateSettings::setDepthTest(bool _b)
    {
        return setFlag(RF_DepthTest, _b);
    }

    RenderStateSettings & RenderStateSettings::setDepthFunction(CompareFunction _func)
    {
        return setField(RF_DepthFuncShift, RF_DepthFuncMask, _func);
    }

    RenderStateSettings & RenderStateSettings::setMultisampling(bool _b)
    {
        return setFlag(RF_Multisampling, _b);
    }

    RenderStateSettings & RenderStateSettings::setDepthWrite(bool _b)
    {
        return setFlag(RF_DepthWrite, _b);
    }

    RenderStateSettings & RenderStateSettings::setColorWrite(bool _r, bool _g, bool _b, bool _a)
    {
        setFlag(RF_ColorWriteRed, _r);
        setFlag(RF_ColorWriteGreen, _g);
        setFlag(RF_ColorWriteBlue, _b);
        return setFlag(RF_ColorWriteAlpha, _a);
    }

    RenderStateSettings & RenderStateSettings::setBlendMode(BlendMode _mode)
    {
        return setBlendMode(_mode, _mode);
    }

    RenderStateSettings & RenderStateSettings::setBlendMode(BlendMode _colorMode, BlendMode _alphaMode)
    {
        setField(RF_ColorBlendModeShift, RF_ColorBlendModeMask, _colorMode);
        return setField(RF_AlphaBlendModeShift, RF_AlphaBlendModeMask, _alphaMode);
    }

    RenderStateSettings & RenderStateSettings::setBlendFunction(BlendFunction _srcFunc, BlendFunction _destFunc)
    {
        return setBlendFunction(_srcFunc, _destFunc, _srcFunc, _destFunc);
    }

    RenderStateSettings & RenderStateSettings::setBlendFunction(BlendFunction _colorSrcFunc, BlendFunction _colorDestFunc,
            BlendFunction _alphaSrcFunc, BlendFunction _alphaDestFunc)
    {
        setField(RF_ColorSourceBlendFuncShift, RF_ColorSourceBlendFuncMask, _colorSrcFunc);
        setField(RF_ColorDestBlendFuncShift, RF_ColorDestBlendFuncMask, _colorDestFunc);
        setField(RF_AlphaSourceBlendFuncShift, RF_AlphaSourceBlendFuncMask, _alphaSrcFunc);
        return setField(RF_AlphaDestBlendFuncShift, RF_AlphaDestBlendFuncMask, _alphaDestFunc);
    }

    RenderStateSettings & RenderStateSettings::setFrontFaceDirection(FaceDirection _dir)
    {
        return setFlag(RF_FrontFaceClockwise, _dir == FaceDirection::Clockwise);
    }

    RenderStateSettings & RenderStateSettings::setCullFace(FaceType _faceType)
    {
        return setField(RF_CullFaceShift, RF_CullFaceMask, _faceType);
    }

    RenderStateSettings & RenderStateSettings::setStencilTest(bool _b)
    {
        return setFlag(RF_StencilTest, _b);
    }

    RenderStateSettings & RenderStateSettings::setStencilFunction(CompareFunction _func, Int32 _ref, UInt32 _mask)
    {
        setStencilFunction(FaceType::Front, _func, _ref, _mask);
        setStencilFunction(FaceType::Back, _func, _ref, _mask);
        //m_stencilState.bFuncSeparate = false;
        return *this;
    }

    RenderStateSettings & RenderStateSettings::setStencilFunction(FaceType _face, CompareFunction _func, Int32 _ref, UInt32 _mask)
    {
        if (_face == FaceType::Front)
        {
            setStencilField(SF_StencilFuncFrontFaceShift, SF_StencilFuncFrontFaceMask, _func);
            m_stencilState.frontFaceRef = _ref;
            m_stencilState.frontFaceMask = _mask;
        }
        else if (_face == FaceType::Back)
        {
            setStencilField(SF_StencilFuncBackFaceShift, SF_StencilFuncBackFaceMask, _func);
            m_stencilState.backFaceRef = _ref;
            m_stencilState.backFaceMask = _mask;
        }
        m_stencilState.bFuncSeparate = true;

        return *this;
    }

    RenderStateSettings & RenderStateSettings::setStencilOperation(StencilOperation _sfail, StencilOperation _dpfail, StencilOperation _dppass)
    {
        setStencilOperation(FaceType::Front, _sfail, _dpfail, _dppass);
        setStencilOperation(FaceType::Back, _sfail, _dpfail, _dppass);
        //m_stencilState.bOpSeparate = false;
        return *this;
    }

    RenderStateSettings & RenderStateSettings::setStencilOperation(FaceType _face, StencilOperation _sfail, StencilOperation _dpfail, StencilOperation _dppass)
    {
        if (_face == FaceType::Front)
        {
            setStencilField(SF_StencilOpSFailFrontFaceShift, SF_StencilOpSFailFrontFaceMask, _sfail);
            setStencilField(SF_StencilOpDPFailFrontFaceShift, SF_StencilOpDPFailFrontFaceMask, _dpfail);
            setStencilField(SF_StencilOpDPPassFrontFaceShift, SF_StencilOpDPPassFrontFaceMask, _dppass);
        }
        else if (_face == FaceType::Back)
        {
            setStencilField(SF_StencilOpSFailBackFaceShift, SF_StencilOpSFailBackFaceMask, _sfail);
            setStencilField(SF_StencilOpDPFailBackFaceShift, SF_StencilOpDPFailBackFaceMask, _dpfail);
            setStencilField(SF_StencilOpDPPassBackFaceShift, SF_StencilOpDPPassBackFaceMask, _dppass);
        }
        m_stencilState.bOpSeparate = true;

        return *this;
    }

    RenderStateSettings & RenderStateSettings::setStencilWrite(UInt32 _mask)
    {
        setStencilWrite(FaceType::Front, _mask);
        setStencilWrite(FaceType::Back, _mask);
        //m_stencilState.bWriteSeparate = false;
        return *this;
    }

    RenderStateSettings & RenderStateSettings::setStencilWrite(FaceType _face, UInt32 _mask)
    {
        if (_face == FaceType::Front)
        {
            m_stencilState.frontFaceWriteMask = _mask;
        }
        else if (_face == FaceType::Back)
        {
            m_stencilState.backFaceWriteMask = _mask;
        }
        m_stencilState.bWriteSeparate = true;
        return *this;
    }

    RenderStateSettings & RenderStateSettings::setFlag(RenderFlag _flag, bool _b)
    {
        if (_b)
            m_renderState |= _flag;
        else
            m_renderState &= ~_flag;
        return *this;
    }

    bool RenderStateSettings::isFlagSet(RenderFlag _flag) const
    {
        return (m_renderState & _flag) == _flag;
    }

    BlendMode RenderStateSettings::colorBlendMode() const
    {
        return field<BlendMode>(RF_ColorBlendModeShift, RF_ColorBlendModeMask);
    }

    BlendMode RenderStateSettings::alphaBlendMode() const
    {
        return field<BlendMode>(RF_AlphaBlendModeShift, RF_AlphaBlendModeMask);
    }

    BlendFunction RenderStateSettings::colorSourceBlendFunction() const
    {
        return field<BlendFunction>(RF_ColorSourceBlendFuncShift, RF_ColorSourceBlendFuncMask);
    }

    BlendFunction RenderStateSettings::colorDestBlendFunction() const
    {
        return field<BlendFunction>(RF_ColorDestBlendFuncShift, RF_ColorDestBlendFuncMask);
    }

    BlendFunction RenderStateSettings::alphaSourceBlendFunction() const
    {
        return field<BlendFunction>(RF_AlphaSourceBlendFuncShift, RF_AlphaSourceBlendFuncMask);
    }

    BlendFunction RenderStateSettings::alphaDestBlendFunction() const
    {
        return field<BlendFunction>(RF_AlphaDestBlendFuncShift, RF_AlphaDestBlendFuncMask);
    }

    FaceType RenderStateSettings::cullFace() const
    {
        return field<FaceType>(RF_CullFaceShift, RF_CullFaceMask);
    }

    CompareFunction RenderStateSettings::stencilFunctionFrontFace() const
    {
        return stencilField<CompareFunction>(SF_StencilFuncFrontFaceShift, SF_StencilFuncFrontFaceMask);
    }

    CompareFunction RenderStateSettings::stencilFunctionBackFace() const
    {
        return stencilField<CompareFunction>(SF_StencilFuncBackFaceShift, SF_StencilFuncBackFaceMask);
    }

    StencilOperation RenderStateSettings::stencilSFailOperationFrontFace() const
    {
        return stencilField<StencilOperation>(SF_StencilOpSFailFrontFaceShift, SF_StencilOpSFailFrontFaceMask);
    }

    StencilOperation RenderStateSettings::stencilDPFailOperationFrontFace() const
    {
        return stencilField<StencilOperation>(SF_StencilOpDPFailFrontFaceShift, SF_StencilOpDPFailFrontFaceMask);
    }

    StencilOperation RenderStateSettings::stencilDPPassOperationFrontFace() const
    {
        return stencilField<StencilOperation>(SF_StencilOpDPPassFrontFaceShift, SF_StencilOpDPPassFrontFaceMask);
    }

    StencilOperation RenderStateSettings::stencilSFailOperationBackFace() const
    {
        return stencilField<StencilOperation>(SF_StencilOpDPFailBackFaceShift, SF_StencilOpDPFailBackFaceMask);
    }

    StencilOperation RenderStateSettings::stencilDPFailOperationBackFace() const
    {
        return stencilField<StencilOperation>(SF_StencilOpDPFailBackFaceShift, SF_StencilOpDPFailBackFaceMask);
    }

    StencilOperation RenderStateSettings::stencilDPPassOperationBackFace() const
    {
        return stencilField<StencilOperation>(SF_StencilOpDPPassBackFaceShift, SF_StencilOpDPPassBackFaceMask);
    }

    UInt64 RenderStateSettings::state() const
    {
        return m_renderState;
    }

    const RenderStateSettings::StencilState & RenderStateSettings::stencilState() const
    {
        return m_stencilState;
    }

    UInt64 RenderStateSettings::differenceMask(UInt64 _mask) const
    {
        return m_renderState ^ _mask;
    }

    UInt64 RenderStateSettings::stencilDifferenceMask(UInt64 _mask) const
    {
        return m_stencilState.state ^ _mask;
    }

    bool RenderStateSettings::isFlagDifferent(UInt64 _diffMask, RenderFlag _flag)
    {
        return (_diffMask & _flag) != 0;
    }

    bool RenderStateSettings::isFieldDifferent(UInt64 _diffMask, UInt64 _fieldMask)
    {
        return (_diffMask & _fieldMask) != 0;
    }

    CompareFunction RenderStateSettings::depthFunction() const
    {
        return field<CompareFunction>(RF_DepthFuncShift, RF_DepthFuncMask);
    }
}
