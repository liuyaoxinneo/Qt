#ifndef DOPPLER_STRUCTURE
#define DOPPLER_STRUCTURE

#pragma pack(8)
struct config_scan
{
    unsigned int eEncoderType  ;// setup_ENCODER_TYPE
    unsigned int eEncoderMode  ;// setup_ENCODER_MODE
    unsigned int ePolarity     ;// setup_ENCODER_POLARITY
    //__attribute__((aligned(8)))
    double fMaxScanSpeed;
    double fResulotion  ;
    double fOrigin      ;

    double fScanStart   ;
    double fScanStop    ;
    double fScanStep    ;
};

#pragma pack(8)
struct config_focallaw
{
    unsigned int eLawType   ;        //  setup_LAW_TYPE
    unsigned int eFocalType ;		 //  setup_FOCAL_TYPE

    unsigned int uiAngleStart;		    // 0.1 degree
    unsigned int uiAngleStop ;			// focal law  angle setting
    unsigned int uiAngleStep ;			//

    unsigned int	uiElem_qty;			    // aperture setting
    unsigned int	uiFirst_tx_elem;		// first trigger element
    unsigned int	uiFirst_rx_elem;		// first receive element
    unsigned int	uiLast_tx_elem;		    // last  trigger element
    unsigned int	uiLast_rx_elem;		    // last  receive element
    unsigned int	uiElem_step;			// element step (linear scan)
    //__attribute__((aligned(8)))
    double	fPositionStart;		            // focal depth
    double	fPositionEnd;		            // not available currently
    double	fPositionStep;		            // not available currently
    double	fOffsetStart;		            // focal plane
    double	fDepthStart;		            //
    double	fOffsetEnd;			            //
    double	fDepthEnd;			            //
} ;
typedef struct
{
    unsigned int  eSkew        ;   //   setup_PROBE_PART_SKEW_ANGLE
    unsigned int   eMaterial    ;  // setup_MATERIAL_TYPE
    double        fThickness  ;  // PART thickness , unit : mm
    double        fScanOffset  ;   //   scan axis offset
    double        fIndexOffset ;   //   index axis offset
} CONFIG_PART_INFO;

#pragma pack(8)
typedef struct
{
    char            strSerial[20]  ;
    char            strMode[20]  ;
    unsigned int    uiElemQty    ;
    //__attribute__((aligned(8)))
    double          fPitch       ;
    double          fFrequency   ;
    double          fReferencePoint;
} CONFIG_PROBE_INFO;

typedef struct
{
    char            strSerial[20] ;
    char            strMode[20]  ;
    unsigned int    eType ;         // 0: PA wedge  1: UT Wedge
    unsigned int    eOrientation;   // 1 Normal 0 reversal//
    double          fAngle;         // wedge angle
    double          fHeight;        // first element height
    double          fProbeDelay;    // UT wedge , unit : ns
    double          fRefPoint;      // UT wedge , unit : mm
    double          fVelocity;      // velocity   m/s
}CONFIG_WEDGE_INFO;


#pragma pack(8)
typedef struct
{
    unsigned int uiStart;       // unit  : ns
    unsigned int uiRange;       // unit  : ns
    unsigned int uiWedgeDelay ;  // unit : ns
    //    __attribute__((aligned(8)))
    double       fGain ;        // range : 0 - 80.0 dB or 0 - 110.0 dB
    double       fVelocity   ;  // unit : m/s
} CONFIG_UT_GENERAL;

#pragma pack(8)
typedef struct
{
    unsigned int uiPulserFir ;  // first trige element 1-128
    unsigned int eTRMode     ;  // setup_TX_RX_MODE
    unsigned int eVoltage    ;  // setup_VOLTAGE_MODE
    unsigned int uiPRF       ;  // pulser repeat frequency
    unsigned int uiPulserWidth ; // unit ns  (align with 2.5)
    //__attribute__((aligned(8)))
    double fFequency  ;  // tirge frequency, unit: MHz
} CONFIG_UT_PULSER;

#pragma pack(8)
typedef struct
{
    unsigned int uiReceiveFir   ;   // first receive element 1-128
    unsigned int eFilterMode    ;   // setup_FILTER_MODE
    unsigned int eRectifier     ;   // setup_RECTIFIER_MODE
    unsigned int bVideoFilter   ;   // video filter : on / off
    unsigned int eAverage       ;   // setup_EVERAGE_MODE
    unsigned int uiReject       ;   // rejection 0 - 100
}CONFIG_UT_RECEIVER;

#pragma pack(8)
typedef struct
{
  unsigned int uiPointQty     ;  //  sample point quantity per law
  //__attribute__((aligned(8)))
  double       fSumGain       ;  //  sum gain , unit : db
} CONFIG_UT_ADVANCED;

typedef struct
{
    unsigned int eGate    ;     // setup_Gate_NAME
    unsigned int uiStart  ;     // gate start position
    unsigned int uiWidth  ;     // gate width (仪器闸门宽度显示的值*2000.0)/(声速/1000.0)
    unsigned int uiThreshold  ; // gate height
    unsigned int eSynChro     ; // setup_GATE_SYN_MODE
    unsigned int eMeasure     ; // setup_GATE_MEASURE_MODE
} CONFIG_GATE_INFO;

typedef struct
{
    unsigned int eDisplay    ;   // setup_DISPLAY_SELECTION_MODE
    unsigned int bDisplayAll ;   // false : display all group , true : display current group
    unsigned int eCScan1     ;   // setup_DISPLAY_CSCAN_SOURCE
    unsigned int eCScan2     ;   // setup_DISPLAY_CSCAN_SOURCE
    unsigned int aeUtUnit[8] ;   // setup_DISPLAY_UTUNIT
}  CONFIG_DISPLAY_INFO;

typedef struct
{
  unsigned int auiBeamDelay[256]    ;  // unit : ns
}CONFIG_GROUP_BEAMDELAY ;

typedef struct
{
    double       afGainOffset[256]    ;  // unit : db
}CONFIG_GROUP_GAINOFFSET;

typedef struct
{
    int                  eCurType ;     /* NONE  ,DAC, LINEAR DAC, TCG */
    int                  nPointQty;     // DAC or TCG setting Point quantity
    int                  nCurveStep ;   // unit 0.01 DB
    int               anRefAmp[256] ;   // unit 0.1 DB
    unsigned int      anAmp[256][16];   // unit 0.01 DB
    unsigned int      anPos[256][16];   // unit 10ns
} CONFIG_SIZING_CURVE;


#endif // DOPPLER_STRUCTURE

