#if !defined(GRANNY_H)
/* ========================================================================
   (C) Copyright 1999-2007 by RAD Game Tools, All Rights Reserved.
   ======================================================================== */
#define GRANNY_32BIT_PTR 0
#define GRANNY_64BIT_PTR 0

#if _XENON
  // Note - the order of these if/elifs is important, since Xenon ALSO defines _XBOX
  #define GRANNY_DYNIMP(ret) ret __stdcall
  #define GRANNY_DYNEXP(ret) ret __stdcall
  #define GRANNY_CALLBACK(ret) ret __cdecl
  #define GRANNY_DYNIMPDATA(type) type
  #define GRANNY_DYNEXPDATA(type) type

  #undef  GRANNY_32BIT_PTR
  #define GRANNY_32BIT_PTR 1
  #define GRANNY_64BIT_TYPE __int64
#elif _XBOX
  #if _XENON
    #error Should never have got here (dll_header.h)
  #endif
  #define GRANNY_DYNIMP(ret) ret __stdcall
  #define GRANNY_DYNEXP(ret) ret __stdcall
  #define GRANNY_CALLBACK(ret) ret __cdecl
  #define GRANNY_DYNIMPDATA(type) type
  #define GRANNY_DYNEXPDATA(type) type
  #undef  GRANNY_32BIT_PTR
  #define GRANNY_32BIT_PTR 1
  #define GRANNY_64BIT_TYPE __int64
#elif _GAMECUBE
  #define GRANNY_DYNIMP(ret) ret
  #define GRANNY_DYNEXP(ret) ret
  #define GRANNY_CALLBACK(ret) ret
  #define GRANNY_DYNIMPDATA(type) type
  #define GRANNY_DYNEXPDATA(type) type
  #undef  GRANNY_32BIT_PTR
  #define GRANNY_32BIT_PTR 1
  #define GRANNY_64BIT_TYPE long long
#elif __CELLOS_LV2__
  // PS3 target (CellOs, not more than one Cello...)
  #define GRANNY_DYNIMP(ret) ret
  #define GRANNY_DYNEXP(ret) ret
  #define GRANNY_CALLBACK(ret) ret
  #define GRANNY_DYNIMPDATA(type) type
  #define GRANNY_DYNEXPDATA(type) type
  #undef  GRANNY_32BIT_PTR
  #define GRANNY_32BIT_PTR 1
  #define GRANNY_64BIT_TYPE long long
#elif _PSX2 || __psp__
  #define GRANNY_DYNIMP(ret) ret
  #define GRANNY_DYNEXP(ret) ret
  #define GRANNY_CALLBACK(ret) ret
  #define GRANNY_DYNIMPDATA(type) type
  #define GRANNY_DYNEXPDATA(type) type
  #undef  GRANNY_32BIT_PTR
  #define GRANNY_32BIT_PTR 1
  #define GRANNY_64BIT_TYPE long
#elif _WIN32
  #if _WIN64
    // Yes, X64 defines both _WIN#2 and _WIN64
    #define GRANNY_DYNIMP(ret) __declspec(dllimport) ret __stdcall
    #define GRANNY_DYNEXP(ret) __declspec(dllexport) ret __stdcall
    #define GRANNY_CALLBACK(ret) ret __cdecl
    #define GRANNY_DYNIMPDATA(type) __declspec(dllimport) type
    #define GRANNY_DYNEXPDATA(type) __declspec(dllexport) type
    #undef  GRANNY_64BIT_PTR
    #define GRANNY_64BIT_PTR 1
  #else
    #define GRANNY_DYNIMP(ret) __declspec(dllimport) ret __stdcall
    #define GRANNY_DYNEXP(ret) __declspec(dllexport) ret __stdcall
    #define GRANNY_CALLBACK(ret) ret __cdecl
    #define GRANNY_DYNIMPDATA(type) __declspec(dllimport) type
    #define GRANNY_DYNEXPDATA(type) __declspec(dllexport) type
    #undef  GRANNY_32BIT_PTR
    #define GRANNY_32BIT_PTR 1
  #endif
  #define GRANNY_64BIT_TYPE __int64
#elif _MACOSX || defined(__MWERKS__)
  #define GRANNY_DYNIMP(ret) ret
  #define GRANNY_DYNEXP(ret) ret
  #define GRANNY_CALLBACK(ret) ret
  #define GRANNY_DYNIMPDATA(type) type
  #define GRANNY_DYNEXPDATA(type) type
  #undef  GRANNY_32BIT_PTR
  #define GRANNY_32BIT_PTR 1
  #define GRANNY_64BIT_TYPE long long

  #if defined(__MWERKS__)
    #pragma export on
  #endif
#elif defined(linux) || defined(__linux__)
  #define GRANNY_DYNIMP(ret) ret
  #define GRANNY_DYNEXP(ret) ret
  #define GRANNY_CALLBACK(ret) ret
  #define GRANNY_DYNIMPDATA(type) type
  #define GRANNY_DYNEXPDATA(type) type
  #undef  GRANNY_32BIT_PTR
  #define GRANNY_32BIT_PTR 1
  #define GRANNY_64BIT_TYPE long long
#endif

/* Switch the import/export state of the functions depending on who's
   compiling, and what compiler we're using */
#if (BUILDING_GRANNY || BUILDING_GRANNY_STATIC)
  #if BUILDING_GRANNY_STATIC
    #define GRANNY_DYNLINK(ret) ret
    #define GRANNY_DYNLINKDATA(type) type
  #else
    #define GRANNY_DYNLINK(ret) GRANNY_DYNEXP(ret)
    #define GRANNY_DYNLINKDATA(type) GRANNY_DYNEXPDATA(type)
  #endif
#else
  #define GRANNY_DYNLINK(ret) GRANNY_DYNIMP(ret)
  #define GRANNY_DYNLINKDATA(type) GRANNY_DYNIMPDATA(type)
#endif

/* Setup our packing directives.  Note that Granny assumes packing of
   its structures.  We ensure that we don't have any alignment less
   than 4-bytes in general */
#ifdef _MSC_VER

  // Byte pack, and disable the warning...
  #pragma pack(push)
  #pragma pack(4)

  #pragma warning(push)
  #pragma warning(disable : 4121) // alignment of a member was sensitive to packing
  #pragma warning(disable : 4103) // used #pragma pack to change alignment
  #define GrannyAlignAttribute

#else

  // Assume gcc style __attribute__ packing directives
  #define GrannyAlignAttribute __attribute__ ((__packed__))

#endif

/* Get the 64 bit types out of the way, and clear our platform macro,
   Note that we have to treat the psp differently, without the DI
   attribute, long is a 32-bit type. */
#if !__psp__
  typedef unsigned GRANNY_64BIT_TYPE granny_uint64x;
  typedef GRANNY_64BIT_TYPE          granny_int64x;
#else
  typedef unsigned long granny_uint64x __attribute__((mode(DI)));
  typedef long granny_int64x __attribute__((mode(DI)));
#endif

#undef GRANNY_64BIT_TYPE  // done with this macro


typedef unsigned int   granny_uint32;
typedef unsigned short granny_uint16;
typedef unsigned char  granny_uint8;

typedef int   granny_int32;
typedef short granny_int16;
typedef char  granny_int8;
typedef int   granny_bool32;

typedef float  granny_real32;
typedef granny_uint16  granny_real16;
// no persistable 64-bit float

typedef granny_uint32 granny_uint32x;
typedef granny_uint32 granny_uint16x;
typedef granny_uint32 granny_uint8x;

typedef granny_int32 granny_int32x;
typedef granny_int32 granny_int16x;
typedef granny_int32 granny_int8x;
typedef granny_int32x granny_bool32x;

typedef double        granny_real64x;
typedef granny_real32 granny_real32x;

typedef granny_real32 granny_triple[3];
typedef granny_real32 granny_quad[4];
typedef granny_real32 granny_matrix_3x3[3][3];
typedef granny_real32 granny_matrix_4x4[4][4];
typedef granny_real32 granny_matrix_3x4[3][4];


// An integer big enough to store a pointer or a memory offset You
// can't store these in files either, hence the "x" on the end.  Note
// that we do a dance here to get the maximum warning level in VC.NET.
// __w64 will spew tons of useful warnings when /Wp64 is turned on.
#if (_MSC_VER >= 1300 && _Wp64)

// Compiles to the correct size on both platforms
  #if GRANNY_32BIT_PTR
    typedef __w64 long granny_intaddrx;
    typedef __w64 unsigned long granny_uintaddrx;
  #elif GRANNY_64BIT_PTR
    typedef __w64 __int64 granny_intaddrx;
    typedef __w64 unsigned __int64 granny_uintaddrx;
  #endif

#else // non-vc.net compiler or /Wp64 turned off

  #if GRANNY_32BIT_PTR
    typedef granny_int32 granny_intaddrx;
    typedef granny_uint32 granny_uintaddrx;
  #elif GRANNY_64BIT_PTR
    typedef granny_int64x granny_intaddrx;
    typedef granny_uint64x granny_uintaddrx;
  #endif

#endif // compiler


// This will fire with a negative subscript error if the type sizes
// are set incorrectly by the compiler, and compile out if not.
#define GrannyTypeSizeCheck(expr) \
    typedef int GrannyTypeSizeCheck__[(expr) ? 1 : -1]

// Exactly sized types
GrannyTypeSizeCheck(sizeof(granny_uint32) == 4);
GrannyTypeSizeCheck(sizeof(granny_uint16) == 2);
GrannyTypeSizeCheck(sizeof(granny_uint8)  == 1);
GrannyTypeSizeCheck(sizeof(granny_int32)  == 4);
GrannyTypeSizeCheck(sizeof(granny_int16)  == 2);
GrannyTypeSizeCheck(sizeof(granny_int8)   == 1);
GrannyTypeSizeCheck(sizeof(granny_real32) == 4);
GrannyTypeSizeCheck(sizeof(granny_bool32) == 4);

// "At least" types
GrannyTypeSizeCheck(sizeof(granny_uint64x) >= 8);
GrannyTypeSizeCheck(sizeof(granny_uint32x) >= 4);
GrannyTypeSizeCheck(sizeof(granny_uint16x) >= 2);
GrannyTypeSizeCheck(sizeof(granny_uint8x)  >= 1);
GrannyTypeSizeCheck(sizeof(granny_int64x)  >= 8);
GrannyTypeSizeCheck(sizeof(granny_int32x)  >= 4);
GrannyTypeSizeCheck(sizeof(granny_int16x)  >= 2);
GrannyTypeSizeCheck(sizeof(granny_int8x)   >= 1);
GrannyTypeSizeCheck(sizeof(granny_real32x) >= 4);
GrannyTypeSizeCheck(sizeof(granny_real64x) >= 8);
GrannyTypeSizeCheck(sizeof(granny_bool32x) >= 4);

// Can hold a pointer
GrannyTypeSizeCheck(sizeof(granny_intaddrx)  == sizeof(void*));
GrannyTypeSizeCheck(sizeof(granny_uintaddrx) == sizeof(void*));

#if !GRANNY_32BIT_PTR && !GRANNY_64BIT_PTR
#error "Must indicate pointer size"
#endif

#if (GRANNY_32BIT_PTR + GRANNY_64BIT_PTR) > 1
#error "Multiple pointer sizes declared"
#endif

#ifdef __cplusplus

// Check that the size of the bool type is what we expected when we
// compiled the library.  If these become decoupled, problems can
// result when the generated code looks at either too much or too
// little for return values.  (Not to mention potential errors in
// array offsets, etc.)  If you're compiling the library yourself on
// an untested compiler, this compile assert may fire.  You simply
// need to ensure that this check matches the one in
// granny_test_types.cpp.
#if (defined(_GAMECUBE) || defined(_PSX2) || (defined(_MACOSX) && defined(__ppc__)))
   GrannyTypeSizeCheck(sizeof(bool) == 4);
#else
   GrannyTypeSizeCheck(sizeof(bool) == 1);
#endif

extern "C" {
#else
    // For C compatibility, we need to define the bool type.  When
    // compiled on non-SN compilers, we expect the type to be 1 byte.
    // For some SN compilers, it's fixed at 4 bytes.  See the note
    // above about compiling the libraries yourself if this causes
    // unexpected problems.  This is tied to the compile-time assert
    // in granny_test_types.cpp if you're compiling the code yourself,
    // and making alterations.
    #if (defined(_GAMECUBE) || defined(_PSX2) || (defined(_MACOSX) && defined(__ppc__)))
        typedef int bool;
    #else
        typedef unsigned char bool;
    #endif
#endif
typedef enum granny_transform_flags
{
    GrannyHasPosition = 0x1,
    GrannyHasOrientation = 0x2,
    GrannyHasScaleShear = 0x4
} granny_transform_flags;

typedef struct granny_transform granny_transform;

typedef struct granny_model_mesh_binding granny_model_mesh_binding;

typedef struct granny_model granny_model;

typedef struct granny_model_control_binding granny_model_control_binding;

typedef struct granny_controlled_animation granny_controlled_animation;

typedef struct granny_controlled_pose granny_controlled_pose;

typedef struct granny_model_instance granny_model_instance;
typedef enum granny_marshalling_type
{
    GrannyAnyMarshalling = 0x0,
    GrannyInt8Marshalling = 0x1,
    GrannyInt16Marshalling = 0x2,
    GrannyInt32Marshalling = 0x4,
    GrannyMarshallingMask = 0x7
} granny_marshalling_type;
typedef enum granny_member_type
{
    GrannyEndMember,
    GrannyInlineMember,
    GrannyReferenceMember,
    GrannyReferenceToArrayMember,
    GrannyArrayOfReferencesMember,
    GrannyVariantReferenceMember,
    GrannyUnsupportedMemberType_Remove,
    GrannyReferenceToVariantArrayMember,
    GrannyStringMember,
    GrannyTransformMember,
    GrannyReal32Member,
    GrannyInt8Member,
    GrannyUInt8Member,
    GrannyBinormalInt8Member,
    GrannyNormalUInt8Member,
    GrannyInt16Member,
    GrannyUInt16Member,
    GrannyBinormalInt16Member,
    GrannyNormalUInt16Member,
    GrannyInt32Member,
    GrannyUInt32Member,
    GrannyReal16Member,
    GrannyEmptyReferenceMember,
    GrannyOnePastLastMemberType,
    GrannyBool32Member = GrannyInt32Member
} granny_member_type;

typedef struct granny_data_type_definition granny_data_type_definition;

typedef struct granny_variant granny_variant;

typedef struct granny_old_curve granny_old_curve;

typedef struct granny_curve2 granny_curve2;

typedef struct granny_curve_data_header granny_curve_data_header;

typedef struct granny_curve_data_da_keyframes32f granny_curve_data_da_keyframes32f;

typedef struct granny_curve_data_da_k32f_c32f granny_curve_data_da_k32f_c32f;

typedef struct granny_curve_data_da_k16u_c16u granny_curve_data_da_k16u_c16u;

typedef struct granny_curve_data_da_k8u_c8u granny_curve_data_da_k8u_c8u;

typedef struct granny_curve_data_d3_k16u_c16u granny_curve_data_d3_k16u_c16u;

typedef struct granny_curve_data_d3_k8u_c8u granny_curve_data_d3_k8u_c8u;

typedef struct granny_curve_data_d4n_k16u_c15u granny_curve_data_d4n_k16u_c15u;

typedef struct granny_curve_data_d4n_k8u_c7u granny_curve_data_d4n_k8u_c7u;

typedef struct granny_curve_data_da_identity granny_curve_data_da_identity;

typedef struct granny_curve_data_da_constant32f granny_curve_data_da_constant32f;

typedef struct granny_curve_data_d3_constant32f granny_curve_data_d3_constant32f;

typedef struct granny_curve_data_d4_constant32f granny_curve_data_d4_constant32f;

typedef struct granny_curve_data_d9i1_k16u_c16u granny_curve_data_d9i1_k16u_c16u;

typedef struct granny_curve_data_d9i3_k16u_c16u granny_curve_data_d9i3_k16u_c16u;

typedef struct granny_curve_data_d9i1_k8u_c8u granny_curve_data_d9i1_k8u_c8u;

typedef struct granny_curve_data_d9i3_k8u_c8u granny_curve_data_d9i3_k8u_c8u;

typedef struct granny_curve_data_d3i1_k32f_c32f granny_curve_data_d3i1_k32f_c32f;

typedef struct granny_curve_data_d3i1_k16u_c16u granny_curve_data_d3i1_k16u_c16u;

typedef struct granny_curve_data_d3i1_k8u_c8u granny_curve_data_d3i1_k8u_c8u;

typedef struct granny_curve_builder granny_curve_builder;
typedef enum granny_material_texture_type
{
    GrannyUnknownTextureType,
    GrannyAmbientColorTexture,
    GrannyDiffuseColorTexture,
    GrannySpecularColorTexture,
    GrannySelfIlluminationTexture,
    GrannyOpacityTexture,
    GrannyBumpHeightTexture,
    GrannyReflectionTexture,
    GrannyRefractionTexture,
    GrannyDisplacementTexture,
    GrannyOnePastLastMaterialTextureType
} granny_material_texture_type;

typedef struct granny_material_map granny_material_map;

typedef struct granny_material granny_material;

typedef struct granny_pixel_layout granny_pixel_layout;

typedef struct granny_animation granny_animation;
typedef enum granny_bound_transform_track_flags
{
    GrannyBoundPositionCurveIsIdentity = (0x0 << 0),
    GrannyBoundPositionCurveIsConstant = (0x1 << 0),
    GrannyBoundPositionCurveIsKeyframed = (0x2 << 0),
    GrannyBoundPositionCurveIsGeneral = (0x3 << 0),
    GrannyBoundPositionCurveFlagMask = (0x3 << 0),
    GrannyBoundOrientationCurveIsIdentity = (0x0 << 2),
    GrannyBoundOrientationCurveIsConstant = (0x1 << 2),
    GrannyBoundOrientationCurveIsKeyframed = (0x2 << 2),
    GrannyBoundOrientationCurveIsGeneral = (0x3 << 2),
    GrannyBoundOrientationCurveFlagMask = (0x3 << 2),
    GrannyBoundScaleShearCurveIsIdentity = (0x0 << 4),
    GrannyBoundScaleShearCurveIsConstant = (0x1 << 4),
    GrannyBoundScaleShearCurveIsKeyframed = (0x2 << 4),
    GrannyBoundScaleShearCurveIsGeneral = (0x3 << 4),
    GrannyBoundScaleShearCurveFlagMask = (0x3 << 4)
} granny_bound_transform_track_flags;

typedef struct granny_bound_transform_track granny_bound_transform_track;

typedef struct granny_animation_binding_identifier granny_animation_binding_identifier;

typedef struct granny_animation_binding granny_animation_binding;

typedef struct granny_animation_lod_builder granny_animation_lod_builder;

typedef struct granny_art_tool_info granny_art_tool_info;
typedef enum granny_transform_file_flags
{
    GrannyRenormalizeNormals = 0x1,
    GrannyReorderTriangleIndices = 0x2
} granny_transform_file_flags;
typedef enum granny_bink_texture_flags
{
    GrannyBinkEncodeAlpha = 0x1,
    GrannyBinkUseScaledRGBInsteadOfYUV = 0x2,
    GrannyBinkUseBink1 = 0x4
} granny_bink_texture_flags;
typedef enum granny_bspline_solver_flags
{
    GrannyBSplineSolverEvaluateAsQuaternions = 0x1,
    GrannyBSplineSolverAllowC0Splitting = 0x2,
    GrannyBSplineSolverAllowC1Splitting = 0x4,
    GrannyBSplineSolverExtraDOFKnotZero = 0x10,
    GrannyBSplineSolverForceEndpointAlignment = 0x20,
    GrannyBSplineSolverAllowReduceKeys = 0x40
} granny_bspline_solver_flags;

typedef struct granny_bspline_solver granny_bspline_solver;

typedef struct granny_bspline_error granny_bspline_error;
#define GrannyInfiniteFarClipPlane 0.0f
#define GrannyLCD17PhysicalAspectRatio 1.25f
#define GrannyNTSCTelevisionPhysicalAspectRatio 1.33f
#define GrannyPALTelevisionPhysicalAspectRatio 1.33f
#define GrannyWidescreenMonitorPhysicalAspectRatio 1.56f
#define GrannyEuropeanFilmAspectRatio 1.66f
#define GrannyUSDigitalTelevisionAspectRatio 1.78f
#define GrannyUSAcademyFlatPhysicalAspectRatio 1.85f
#define GrannyUSPanavisionAspectRatio 2.20f
#define GrannyUSAnamorphicScopePhysicalAspectRatio 2.35f
typedef enum granny_camera_output_z_range
{
    GrannyCameraOutputZZeroToOne,
    GrannyCameraOutputZNegativeOneToOne,
    GrannyCameraOutputZNegativeOneToZero
} granny_camera_output_z_range;

typedef struct granny_camera granny_camera;

typedef struct granny_compress_curve_parameters granny_compress_curve_parameters;

typedef struct granny_control granny_control;
typedef enum granny_accumulation_mode
{
    GrannyNoAccumulation,
    GrannyConstantExtractionAccumulation,
    GrannyVariableDeltaAccumulation
} granny_accumulation_mode;

typedef struct granny_controlled_animation_builder granny_controlled_animation_builder;
typedef enum granny_blend_dag_node_type
{
    GrannyDagNodeType_Leaf_AnimationBlend,
    GrannyDagNodeType_Leaf_LocalPose,
    GrannyDagNodeType_Leaf_Callback,
    GrannyDagNodeType_OnePastLastLeafType,
    GrannyDagNodeType_Node_Crossfade,
    GrannyDagNodeType_Node_WeightedBlend,
    GrannyDagNodeType_OnePastLast
} granny_blend_dag_node_type;

typedef struct granny_blend_dag_node granny_blend_dag_node;

typedef struct granny_dag_pose_cache granny_dag_pose_cache;
typedef enum granny_file_data_tree_flags
{
    GrannyExcludeTypeTree = 0x1
} granny_file_data_tree_flags;

typedef struct granny_file_data_tree_writer granny_file_data_tree_writer;
typedef enum granny_deformation_type
{
    GrannyDeformPosition = 1,
    GrannyDeformPositionNormal,
    GrannyDeformPositionNormalTangent,
    GrannyDeformPositionNormalTangentBinormal
} granny_deformation_type;
typedef enum granny_degree_of_freedom
{
    GrannyNoDOFs = 0,
    GrannyXTranslation = 0x001,
    GrannyYTranslation = 0x002,
    GrannyZTranslation = 0x004,
    GrannyXRotation = 0x008,
    GrannyYRotation = 0x010,
    GrannyZRotation = 0x020,
    GrannyXScaleShear = 0x040,
    GrannyYScaleShear = 0x080,
    GrannyZScaleShear = 0x100,
    GrannyTranslationDOFs = GrannyXTranslation | GrannyYTranslation | GrannyZTranslation,
    GrannyRotationDOFs = GrannyXRotation | GrannyYRotation | GrannyZRotation,
    GrannyScaleShearDOFs = GrannyXScaleShear | GrannyYScaleShear | GrannyZScaleShear,
    GrannyAllDOFs = GrannyTranslationDOFs | GrannyRotationDOFs | GrannyScaleShearDOFs
} granny_degree_of_freedom;

typedef struct granny_exporter_info granny_exporter_info;

typedef struct granny_file granny_file;

typedef struct granny_file_location granny_file_location;

typedef struct granny_file_fixup granny_file_fixup;

typedef struct granny_file_builder granny_file_builder;
typedef enum granny_compression_type
{
    GrannyNoCompression,
    GrannyOodle0Compression,
    GrannyOodle1Compression,
    GrannyOnePastLastCompressionType
} granny_compression_type;

typedef struct granny_file_compressor granny_file_compressor;
#define GrannyCurrentGRNStandardTag (0x80000000 + 38)
#define GrannyCurrentGRNFileVersion 7
#define GrannyGRNExtraTagCount 4
typedef enum granny_standard_section_index
{
    GrannyStandardMainSection = 0,
    GrannyStandardRigidVertexSection = 1,
    GrannyStandardRigidIndexSection = 2,
    GrannyStandardDeformableVertexSection = 3,
    GrannyStandardDeformableIndexSection = 4,
    GrannyStandardTextureSection = 5,
    GrannyStandardDiscardableSection = 6,
    GrannyStandardUnloadedSection = 7,
    GrannyStandardSectionCount
} granny_standard_section_index;
typedef enum granny_grn_type_tag
{
    GrannyFirstGRNUserTag = 0,
    GrannyLastGRNUserTag = 0x7FFFFFFF,
    GrannyFirstGRNStandardTag = 0x80000000,
    GrannyLastGRNStandardTag = 0xFFFFFFFF
} granny_grn_type_tag;

typedef struct granny_grn_file_magic_value granny_grn_file_magic_value;

typedef struct granny_grn_reference granny_grn_reference;

typedef struct granny_grn_pointer_fixup granny_grn_pointer_fixup;

typedef struct granny_grn_mixed_marshalling_fixup granny_grn_mixed_marshalling_fixup;

typedef struct granny_grn_section granny_grn_section;

typedef struct granny_grn_file_header granny_grn_file_header;

typedef struct granny_file_info granny_file_info;
#define GrannyCloseFileReader(Reader) if(Reader) {(*(Reader)->CloseFileReaderCallback)(__FILE__, __LINE__, Reader);}
#define GrannyReadAtMost(Reader, Pos, Count, Buffer) (*(Reader).ReadAtMostCallback)(__FILE__, __LINE__, Reader, Pos, Count, Buffer)
#define GrannyReadExactly(Reader, Pos, Count, Buffer) ((*(Reader).ReadAtMostCallback)(__FILE__, __LINE__, Reader, Pos, Count, Buffer) == Count)

typedef struct granny_file_reader granny_file_reader;
#define GrannyDeleteFileWriter(Writer) (*(Writer)->DeleteFileWriterCallback)(__FILE__, __LINE__, Writer)
#define GrannyGetWriterPosition(Writer) (*(Writer).SeekWriterCallback)(__FILE__, __LINE__, Writer, 0, SeekCurrent)
#define GrannySeekWriterFromStart(Writer, OffsetInUInt8s) (SeekWriterFromStartStub(__FILE__, __LINE__, Writer, OffsetInUInt8s))
#define GrannySeekWriterFromEnd(Writer, OffsetInUInt8s) (SeekWriterFromEndStub(__FILE__, __LINE__, Writer, OffsetInUInt8s))
#define GrannySeekWriterFromCurrentPosition(Writer, OffsetInUInt8s) (SeekWriterFromCurrentPositionStub(__FILE__, __LINE__, Writer, OffsetInUInt8s))
#define GrannyWrite(Writer, UInt8Count, WritePointer) (*(Writer).WriteCallback)(__FILE__, __LINE__, Writer, UInt8Count, WritePointer)
#define GrannyBeginWriterCRC(Writer) (*(Writer).BeginCRCCallback)(__FILE__, __LINE__, Writer)
#define GrannyEndWriterCRC(Writer) (*(Writer).EndCRCCallback)(__FILE__, __LINE__, Writer)
#define GrannyWriterIsCRCing(Writer) ((Writer).CRCing)
typedef enum granny_file_writer_seek_type
{
    GrannySeekStart,
    GrannySeekEnd,
    GrannySeekCurrent
} granny_file_writer_seek_type;

typedef struct granny_file_writer granny_file_writer;

typedef struct granny_fixed_allocator_unit granny_fixed_allocator_unit;

typedef struct granny_fixed_allocator_block granny_fixed_allocator_block;

typedef struct granny_fixed_allocator granny_fixed_allocator;
typedef enum granny_pixel_filter_type
{
    GrannyCubicPixelFilter,
    GrannyLinearPixelFilter,
    GrannyBoxPixelFilter,
    GrannyOnePastLastPixelFilterType
} granny_pixel_filter_type;

typedef struct granny_box_intersection granny_box_intersection;

typedef struct granny_triangle_intersection granny_triangle_intersection;
#define GrannyMaximumSystemFileNameSize (1 << 9)
#define GrannyMaximumTempFileNameSize (1 << 9)
#define GrannyMaximumMessageBufferSize (1 << 15)
#define GrannyMaximumLogFileNameSize (1 << 9)
#define GrannyMaximumAggregateCount (1 << 6)
#define GrannyMaximumNumberToStringBuffer (1 << 8)
#define GrannyMaximumIKLinkCount (1 << 8)
#define GrannyMaximumMIPLevelCount (1 << 5)
#define GrannyMaximumVertexNameLength (1 << 6)
#define GrannyMaximumUserData (1 << 2)
#define GrannyMaximumBoneNameLength (1 << 9)
#define GrannyMaximumChannelCount (1 << 8)
#define GrannyMaximumWeightCount (1 << 8)
#define GrannyMaximumChannelWidth (1 << 2)
#define GrannyMaximumBSplineDimension 16
#define GrannyMaximumBSplineKnotPower 16
#define GrannyDefaultAllocationAlignment 4
#define GrannyMatrixBufferAlignment 16
#define GrannyDefaultFixedAllocatorBlockAlignment 16
#define GrannyBlockFileFixupCount (1 << 8)
#define GrannyExpectedUsablePageSize (4000)
#define GrannyMinimumAllocationsPerFixedBlock (1 << 6)
#define GrannyFileCopyBufferSize (1 << 16)
#define GrannyCRCCheckBufferSize (1 << 16)
#define GrannyTrackWeightEpsilon 0.001f
#define GrannyPositionIdentityThreshold 0.001f
#define GrannyOrientationIdentityThreshold 0.0001f
#define GrannyScaleShearIdentityThreshold 0.001f
#define GrannyBlendEffectivelyZero 0.001f
#define GrannyBlendEffectivelyOne 0.999f
#define GrannyTimeEffectivelyZero 0.00001f
#define GrannyDefaultLocalPoseFillThreshold 0.2f
#define GrannyNoSparseBone -1
typedef enum granny_quaternion_mode
{
    GrannyBlendQuaternionDirectly = 0,
    GrannyBlendQuaternionInverted = 1,
    GrannyBlendQuaternionNeighborhooded = 2,
    GrannyBlendQuaternionAccumNeighborhooded = 3
} granny_quaternion_mode;

typedef struct granny_local_pose granny_local_pose;
typedef enum granny_log_message_type
{
    GrannyIgnoredLogMessage,
    GrannyNoteLogMessage,
    GrannyWarningLogMessage,
    GrannyErrorLogMessage,
    GrannyOnePastLastMessageType
} granny_log_message_type;
typedef enum granny_log_message_origin
{
    GrannyNotImplementedLogMessage,
    GrannyApplicationLogMessage,
    GrannyWin32SubsystemLogMessage,
    GrannyWin64SubsystemLogMessage,
    GrannyMacOSSubsystemLogMessage,
    GrannyANSISubsystemLogMessage,
    GrannyGamecubeSubsystemLogMessage,
    GrannyPS2SubsystemLogMessage,
    GrannyPSPSubsystemLogMessage,
    GrannyPS3SubsystemLogMessage,
    GrannyXboxSubsystemLogMessage,
    GrannyXenonSubsystemLogMessage,
    GrannyMAXSubsystemLogMessage,
    GrannyMayaSubsystemLogMessage,
    GrannyXSISubsystemLogMessage,
    GrannyLightwaveSubsystemLogMessage,
    GrannyFileWritingLogMessage,
    GrannyFileReadingLogMessage,
    GrannyExporterLogMessage,
    GrannyCompressorLogMessage,
    GrannyStringLogMessage,
    GrannyStringTableLogMessage,
    GrannyVertexLayoutLogMessage,
    GrannyMeshLogMessage,
    GrannyPropertyLogMessage,
    GrannySkeletonLogMessage,
    GrannyAnimationLogMessage,
    GrannySetupGraphLogMessage,
    GrannyTextureLogMessage,
    GrannyBSplineLogMessage,
    GrannyHashLogMessage,
    GrannyLinkerLogMessage,
    GrannyInstantiatorLogMessage,
    GrannyDataTypeLogMessage,
    GrannyNameMapLogMessage,
    GrannyMaterialLogMessage,
    GrannyModelLogMessage,
    GrannyStackAllocatorLogMessage,
    GrannyFixedAllocatorLogMessage,
    GrannySceneLogMessage,
    GrannyTrackMaskLogMessage,
    GrannyLocalPoseLogMessage,
    GrannyWorldPoseLogMessage,
    GrannyNameLibraryLogMessage,
    GrannyControlLogMessage,
    GrannyMeshBindingLogMessage,
    GrannyMathLogMessage,
    GrannyVersionLogMessage,
    GrannyMemoryLogMessage,
    GrannyDeformerLogMessage,
    GrannyVoxelLogMessage,
    GrannyBitmapLogMessage,
    GrannyIKLogMessage,
    GrannyCurveLogMessage,
    GrannyTrackGroupLogMessage,
    GrannyThreadSafetyLogMessage,
    GrannyQuantizeLogMessage,
    GrannyBlendDagLogMessage,
    GrannyOnePastLastMessageOrigin
} granny_log_message_origin;

typedef struct granny_log_callback granny_log_callback;

typedef struct granny_allocation_header granny_allocation_header;

typedef struct granny_allocation_information granny_allocation_information;

typedef struct granny_memory_arena granny_memory_arena;

typedef struct granny_bone_binding granny_bone_binding;

typedef struct granny_material_binding granny_material_binding;

typedef struct granny_morph_target granny_morph_target;

typedef struct granny_mesh granny_mesh;

typedef struct granny_mesh_binding granny_mesh_binding;

typedef struct granny_mesh_builder granny_mesh_builder;
typedef enum granny_deformer_tail_flags
{
    GrannyDontAllowUncopiedTail,
    GrannyAllowUncopiedTail
} granny_deformer_tail_flags;

typedef struct granny_mesh_deformer granny_mesh_deformer;

typedef struct granny_oodle1_state granny_oodle1_state;

typedef struct granny_periodic_loop granny_periodic_loop;

typedef struct granny_pointer_hash granny_pointer_hash;
typedef enum granny_s3tc_texture_format
{
    GrannyS3TCBGR565,
    GrannyS3TCBGRA5551,
    GrannyS3TCBGRA8888MappedAlpha,
    GrannyS3TCBGRA8888InterpolatedAlpha,
    GrannyOnePastLastS3TCTextureFormat
} granny_s3tc_texture_format;
#define GrannyNoParentBone -1
typedef enum granny_skeleton_lod_type
{
    GrannyNoSkeletonLOD = 0x0,
    GrannyEstimatedLOD = 0x1,
    GrannyMeasuredLOD = 0x2
} granny_skeleton_lod_type;

typedef struct granny_bone granny_bone;

typedef struct granny_skeleton granny_skeleton;

typedef struct granny_skeleton_builder granny_skeleton_builder;

typedef struct granny_spu_animation granny_spu_animation;

typedef struct granny_spu_animation_binding_id granny_spu_animation_binding_id;

typedef struct granny_spu_animation_binding granny_spu_animation_binding;

typedef struct granny_spu_animation_info granny_spu_animation_info;

typedef struct granny_controlled_spu_animation_builder granny_controlled_spu_animation_builder;
typedef enum granny_spu_curve_types
{
    GrannySPUCurveTypeReal32 = 0,
    GrannySPUCurveTypeK16 = 1,
    GrannySPUCurveTypeK8 = 2,
    GrannySPUCurveType4nK16 = 3,
    GrannySPUCurveType4nK8 = 4
} granny_spu_curve_types;
typedef enum granny_spu_replication_type
{
    GrannySPUReplicationRaw = 0,
    GrannySPUReplicationNormOri = 1,
    GrannySPUReplicationDiagonalSS = 2
} granny_spu_replication_type;

typedef struct granny_spu_curve_header_basic granny_spu_curve_header_basic;

typedef struct granny_spu_curve_header_quantized granny_spu_curve_header_quantized;

typedef struct granny_spu_curve_header_quantized4n granny_spu_curve_header_quantized4n;
#define GrannySPUTransformTrackNoCurve (0xffffffff)

typedef struct granny_spu_transform_track granny_spu_transform_track;

typedef struct granny_spu_track_group granny_spu_track_group;

typedef struct granny_allocated_block granny_allocated_block;

typedef struct granny_stack_allocator granny_stack_allocator;

typedef struct granny_counter_results granny_counter_results;

typedef struct granny_animation_binding_cache_status granny_animation_binding_cache_status;

typedef struct granny_stat_hud_alloc_point granny_stat_hud_alloc_point;

typedef struct granny_stat_hud_perf_point granny_stat_hud_perf_point;

typedef struct granny_stat_hud_model_instances granny_stat_hud_model_instances;

typedef struct granny_stat_hud_model_controls granny_stat_hud_model_controls;

typedef struct granny_stat_hud_footprint granny_stat_hud_footprint;

typedef struct granny_stat_hud_perf granny_stat_hud_perf;

typedef struct granny_stat_hud_animation_types granny_stat_hud_animation_types;

typedef struct granny_stat_hud granny_stat_hud;

typedef struct granny_string_database granny_string_database;

typedef struct granny_string_table granny_string_table;

typedef struct granny_system_clock granny_system_clock;

typedef struct granny_tangent_frame granny_tangent_frame;
typedef enum granny_texture_type
{
    GrannyColorMapTextureType,
    GrannyCubeMapTextureType,
    GrannyOnePastLastTextureType
} granny_texture_type;
typedef enum granny_texture_encoding
{
    GrannyUserTextureEncoding,
    GrannyRawTextureEncoding,
    GrannyS3TCTextureEncoding,
    GrannyBinkTextureEncoding,
    GrannyOnePastLastTextureEncoding
} granny_texture_encoding;

typedef struct granny_texture_mip_level granny_texture_mip_level;

typedef struct granny_texture_image granny_texture_image;

typedef struct granny_texture granny_texture;

typedef struct granny_texture_builder granny_texture_builder;
typedef enum granny_transform_track_flags
{
    GrannyUseAccumulatorNeighborhood = 0x1
} granny_transform_track_flags;
typedef enum granny_track_group_flags
{
    GrannyAccumulationExtracted = 0x1,
    GrannyTrackGroupIsSorted = 0x2,
    GrannyAccumulationIsVDA = 0x4
} granny_track_group_flags;
typedef enum granny_vector_diff_mode
{
    GrannyDiffAsVectors,
    GrannyDiffAsQuaternions,
    GrannyManhattanMetric
} granny_vector_diff_mode;

typedef struct granny_vector_track granny_vector_track;

typedef struct granny_transform_track granny_transform_track;

typedef struct granny_text_track_entry granny_text_track_entry;

typedef struct granny_text_track granny_text_track;

typedef struct granny_track_group granny_track_group;

typedef struct granny_track_group_builder granny_track_group_builder;

typedef struct granny_track_group_sampler granny_track_group_sampler;
typedef enum granny_extract_track_mask_result
{
    GrannyExtractTrackMaskResult_AllDataPresent,
    GrannyExtractTrackMaskResult_PartialDataPresent,
    GrannyExtractTrackMaskResult_NoDataPresent
} granny_extract_track_mask_result;

typedef struct granny_track_mask granny_track_mask;

typedef struct granny_unbound_weight granny_unbound_weight;

typedef struct granny_unbound_track_mask granny_unbound_track_mask;

typedef struct granny_sample_context granny_sample_context;
#define GrannyTopologyNullIndex 0xFFFFFFFF

typedef struct granny_tri_material_group granny_tri_material_group;

typedef struct granny_tri_annotation_set granny_tri_annotation_set;

typedef struct granny_tri_topology granny_tri_topology;

typedef struct granny_defined_type granny_defined_type;

typedef struct granny_variant_builder granny_variant_builder;
#define GrannyProductVersion "2.7.0.30"
#define GrannyProductMajorVersion 2
#define GrannyProductMinorVersion 7
#define GrannyProductCustomization 0
#define GrannyProductBuildNumber 30
#define GrannyProductReleaseName release
#define GrannyProductCompanyName "RAD Game Tools, Inc."
#define GrannyProductName "Granny"
#define GrannyProductCopyright "1999-2007 by RAD Game Tools, Inc., All Rights Reserved."
#define GrannyProductTrademarks ProductName " is a registered trademark of " ProductCompanyName
#define GrannyProductComments "Who's your Granny?"
#define GrannyProductSupportAddress "granny3@radgametools.com"
#define GrannyProductSupportPage "www.radgametools.com/granny.html"
#define GrannyVersionsMatch GrannyVersionsMatch_(GrannyProductMajorVersion, GrannyProductMinorVersion, GrannyProductCustomization, GrannyProductBuildNumber)
#define GrannyVertexPositionName "Position"
#define GrannyVertexNormalName "Normal"
#define GrannyVertexTangentName "Tangent"
#define GrannyVertexBinormalName "Binormal"
#define GrannyVertexTangentBinormalCrossName "TangentBinormalCross"
#define GrannyVertexBoneWeightsName "BoneWeights"
#define GrannyVertexBoneIndicesName "BoneIndices"
#define GrannyVertexDiffuseColorName "DiffuseColor"
#define GrannyVertexSpecularColorName "SpecularColor"
#define GrannyVertexTextureCoordinatesName "TextureCoordinates"
#define GrannyVertexMorphCurvePrefix "VertexMorphCurve"

typedef struct granny_vertex_annotation_set granny_vertex_annotation_set;

typedef struct granny_vertex_data granny_vertex_data;

typedef struct granny_p3_vertex granny_p3_vertex;

typedef struct granny_pt32_vertex granny_pt32_vertex;

typedef struct granny_pn33_vertex granny_pn33_vertex;

typedef struct granny_png333_vertex granny_png333_vertex;

typedef struct granny_pngt3332_vertex granny_pngt3332_vertex;

typedef struct granny_pntg3323_vertex granny_pntg3323_vertex;

typedef struct granny_pngb3333_vertex granny_pngb3333_vertex;

typedef struct granny_pnt332_vertex granny_pnt332_vertex;

typedef struct granny_pngbt33332_vertex granny_pngbt33332_vertex;

typedef struct granny_pnt333_vertex granny_pnt333_vertex;

typedef struct granny_pngbt33333_vertex granny_pngbt33333_vertex;

typedef struct granny_pwn313_vertex granny_pwn313_vertex;

typedef struct granny_pwng3133_vertex granny_pwng3133_vertex;

typedef struct granny_pwngt31332_vertex granny_pwngt31332_vertex;

typedef struct granny_pwngb31333_vertex granny_pwngb31333_vertex;

typedef struct granny_pwnt3132_vertex granny_pwnt3132_vertex;

typedef struct granny_pwngbt313332_vertex granny_pwngbt313332_vertex;

typedef struct granny_pwn323_vertex granny_pwn323_vertex;

typedef struct granny_pwng3233_vertex granny_pwng3233_vertex;

typedef struct granny_pwngt32332_vertex granny_pwngt32332_vertex;

typedef struct granny_pwngb32333_vertex granny_pwngb32333_vertex;

typedef struct granny_pwnt3232_vertex granny_pwnt3232_vertex;

typedef struct granny_pwngbt323332_vertex granny_pwngbt323332_vertex;

typedef struct granny_pwn343_vertex granny_pwn343_vertex;

typedef struct granny_pwng3433_vertex granny_pwng3433_vertex;

typedef struct granny_pwngt34332_vertex granny_pwngt34332_vertex;

typedef struct granny_pwngb34333_vertex granny_pwngb34333_vertex;

typedef struct granny_pwnt3432_vertex granny_pwnt3432_vertex;

typedef struct granny_pwngbt343332_vertex granny_pwngbt343332_vertex;

typedef struct granny_vertex_weight_arrays granny_vertex_weight_arrays;
typedef enum granny_composite_flag
{
    GrannyIncludeComposites = 0,
    GrannyExcludeComposites = 1
} granny_composite_flag;

typedef struct granny_world_pose granny_world_pose;
typedef GRANNY_CALLBACK(granny_local_pose *) granny_blend_dag_leaf_callback_sample_callback(void * UserData,
                                                                                            granny_int32x BoneCount,
                                                                                            granny_real32 AllowedError,
                                                                                            granny_int32x const * SparseBoneArray);
typedef GRANNY_CALLBACK(void) granny_blend_dag_leaf_callback_set_clock_callback(void * UserData,
                                                                                granny_real32 NewClock);
typedef GRANNY_CALLBACK(void) granny_blend_dag_leaf_callback_motion_vectors_callback(void * UserData,
                                                                                     granny_real32 SecondsElapsed,
                                                                                     granny_real32 * ResultTranslationVec3,
                                                                                     granny_real32 * ResultRotationVec3,
                                                                                     bool Inverse);
typedef GRANNY_CALLBACK(char *) granny_rebase_pointers_string_callback(void * Data,
                                                                       granny_uint32 Identifier);
typedef GRANNY_CALLBACK(granny_uint32) granny_file_writer_string_callback(void * Data,
                                                                          char const * String);
typedef GRANNY_CALLBACK(granny_file_reader *) granny_open_file_reader_callback(char const * SourceFileName,
                                                                               granny_int32x SourceLineNumber,
                                                                               char const * FileName);
typedef GRANNY_CALLBACK(void) granny_close_file_reader_callback(char const * SourceFileName,
                                                                granny_int32x SourceLineNumber,
                                                                granny_file_reader * Reader);
typedef GRANNY_CALLBACK(granny_int32x) granny_read_at_most_callback(char const * SourceFileName,
                                                                    granny_int32x SourceLineNumber,
                                                                    granny_file_reader * Reader,
                                                                    granny_int32x FilePosition,
                                                                    granny_int32x UInt8Count,
                                                                    void * Buffer);
typedef GRANNY_CALLBACK(granny_file_writer *) granny_open_file_writer_callback(char const * SourceFileName,
                                                                               granny_int32x SourceLineNumber,
                                                                               char const * FileName,
                                                                               bool EraseExisting);
typedef GRANNY_CALLBACK(void) granny_delete_file_writer_callback(char const * SourceFileName,
                                                                 granny_int32x SourceLineNumber,
                                                                 granny_file_writer * Writer);
typedef GRANNY_CALLBACK(granny_int32x) granny_seek_file_writer_callback(char const * SourceFileName,
                                                                        granny_int32x SourceLineNumber,
                                                                        granny_file_writer * Writer,
                                                                        granny_int32x OffsetInUInt8s,
                                                                        granny_file_writer_seek_type SeekType);
typedef GRANNY_CALLBACK(bool) granny_write_file_writer_callback(char const * SourceFileName,
                                                                granny_int32x SourceLineNumber,
                                                                granny_file_writer * Writer,
                                                                granny_int32x UInt8Count,
                                                                void const * WritePointer);
typedef GRANNY_CALLBACK(void) granny_begincrc_file_writer_callback(char const * SourceFileName,
                                                                   granny_int32x SourceLineNumber,
                                                                   granny_file_writer * Writer);
typedef GRANNY_CALLBACK(granny_uint32) granny_endcrc_file_writer_callback(char const * SourceFileName,
                                                                          granny_int32x SourceLineNumber,
                                                                          granny_file_writer * Writer);
typedef GRANNY_CALLBACK(void) granny_log_function(granny_log_message_type Type,
                                                  granny_log_message_origin Origin,
                                                  char const * Message,
                                                  void * UserData);
typedef GRANNY_CALLBACK(void *) granny_allocate_callback(char const * File,
                                                         granny_int32x Line,
                                                         granny_intaddrx Alignment,
                                                         granny_intaddrx Size);
typedef GRANNY_CALLBACK(void) granny_deallocate_callback(char const * File,
                                                         granny_int32x Line,
                                                         void * Memory);
typedef GRANNY_CALLBACK(granny_int32x) granny_string_comparison_callback(char const * A,
                                                                         char const * B);
typedef GRANNY_CALLBACK(granny_uint32x) granny_thread_id_callback(void);
typedef GRANNY_CALLBACK(void) granny_track_sampler(granny_sample_context const * Context,
                                                   granny_transform_track const * SourceTrack,
                                                   granny_bound_transform_track * Track,
                                                   granny_transform const * RestTransform,
                                                   granny_real32 const * InverseWorld4x4,
                                                   granny_real32 const * ParentMatrix,
                                                   granny_real32 * WorldResult,
                                                   granny_real32 * CompositeResult);

struct granny_transform
{
    granny_uint32 Flags;
    granny_triple Position;
    granny_quad Orientation;
    granny_triple ScaleShear[3];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_transform) == (sizeof(granny_uint32) + 
                                                 sizeof(granny_triple) + 
                                                 sizeof(granny_quad) + 
                                                 sizeof(granny_triple) * 3));

struct granny_model_mesh_binding
{
    granny_mesh * Mesh;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_model_mesh_binding) == (sizeof(granny_mesh *)));

struct granny_model
{
    char const * Name;
    granny_skeleton * Skeleton;
    granny_transform InitialPlacement;
    granny_int32 MeshBindingCount;
    granny_model_mesh_binding * MeshBindings;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_model) == (sizeof(char const *) + 
                                             sizeof(granny_skeleton *) + 
                                             sizeof(granny_transform) + 
                                             sizeof(granny_int32) + 
                                             sizeof(granny_model_mesh_binding *)));

struct granny_model_control_binding;

struct granny_controlled_animation;

struct granny_controlled_pose;

struct granny_model_instance;

struct granny_data_type_definition
{
    granny_member_type Type;
    char const * Name;
    granny_data_type_definition * ReferenceType;
    granny_int32 ArrayWidth;
    granny_int32 Extra[3];
    granny_uintaddrx Ignored__Ignored;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_data_type_definition) == (sizeof(granny_member_type) + 
                                                            sizeof(char const *) + 
                                                            sizeof(granny_data_type_definition *) + 
                                                            sizeof(granny_int32) + 
                                                            sizeof(granny_int32) * 3 + 
                                                            sizeof(granny_uintaddrx)));

struct granny_variant
{
    granny_data_type_definition * Type;
    void * Object;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_variant) == (sizeof(granny_data_type_definition *) + 
                                               sizeof(void *)));

struct granny_old_curve
{
    granny_int32 Degree;
    granny_int32 KnotCount;
    granny_real32 * Knots;
    granny_int32 ControlCount;
    granny_real32 * Controls;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_old_curve) == (sizeof(granny_int32) + 
                                                 sizeof(granny_int32) + 
                                                 sizeof(granny_real32 *) + 
                                                 sizeof(granny_int32) + 
                                                 sizeof(granny_real32 *)));

struct granny_curve2
{
    granny_variant CurveData;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_curve2) == (sizeof(granny_variant)));

struct granny_curve_data_header
{
    granny_uint8 Format;
    granny_uint8 Degree;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_curve_data_header) == (sizeof(granny_uint8) + 
                                                         sizeof(granny_uint8)));

struct granny_curve_data_da_keyframes32f
{
    granny_curve_data_header CurveDataHeader;
    granny_int16 Dimension;
    granny_int32 ControlCount;
    granny_real32 * Controls;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_curve_data_da_keyframes32f) == (sizeof(granny_curve_data_header) + 
                                                                  sizeof(granny_int16) + 
                                                                  sizeof(granny_int32) + 
                                                                  sizeof(granny_real32 *)));

struct granny_curve_data_da_k32f_c32f
{
    granny_curve_data_header CurveDataHeader;
    granny_int16 Padding;
    granny_int32 KnotCount;
    granny_real32 * Knots;
    granny_int32 ControlCount;
    granny_real32 * Controls;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_curve_data_da_k32f_c32f) == (sizeof(granny_curve_data_header) + 
                                                               sizeof(granny_int16) + 
                                                               sizeof(granny_int32) + 
                                                               sizeof(granny_real32 *) + 
                                                               sizeof(granny_int32) + 
                                                               sizeof(granny_real32 *)));

struct granny_curve_data_da_k16u_c16u
{
    granny_curve_data_header CurveDataHeader;
    granny_uint16 OneOverKnotScaleTrunc;
    granny_int32 ControlScaleOffsetCount;
    granny_real32 * ControlScaleOffsets;
    granny_int32 KnotControlCount;
    granny_uint16 * KnotsControls;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_curve_data_da_k16u_c16u) == (sizeof(granny_curve_data_header) + 
                                                               sizeof(granny_uint16) + 
                                                               sizeof(granny_int32) + 
                                                               sizeof(granny_real32 *) + 
                                                               sizeof(granny_int32) + 
                                                               sizeof(granny_uint16 *)));

struct granny_curve_data_da_k8u_c8u
{
    granny_curve_data_header CurveDataHeader;
    granny_uint16 OneOverKnotScaleTrunc;
    granny_int32 ControlScaleOffsetCount;
    granny_real32 * ControlScaleOffsets;
    granny_int32 KnotControlCount;
    granny_uint8 * KnotsControls;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_curve_data_da_k8u_c8u) == (sizeof(granny_curve_data_header) + 
                                                             sizeof(granny_uint16) + 
                                                             sizeof(granny_int32) + 
                                                             sizeof(granny_real32 *) + 
                                                             sizeof(granny_int32) + 
                                                             sizeof(granny_uint8 *)));

struct granny_curve_data_d3_k16u_c16u
{
    granny_curve_data_header CurveDataHeader;
    granny_uint16 OneOverKnotScaleTrunc;
    granny_real32 ControlScales[3];
    granny_real32 ControlOffsets[3];
    granny_int32 KnotControlCount;
    granny_uint16 * KnotsControls;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_curve_data_d3_k16u_c16u) == (sizeof(granny_curve_data_header) + 
                                                               sizeof(granny_uint16) + 
                                                               sizeof(granny_real32) * 3 + 
                                                               sizeof(granny_real32) * 3 + 
                                                               sizeof(granny_int32) + 
                                                               sizeof(granny_uint16 *)));

struct granny_curve_data_d3_k8u_c8u
{
    granny_curve_data_header CurveDataHeader;
    granny_uint16 OneOverKnotScaleTrunc;
    granny_real32 ControlScales[3];
    granny_real32 ControlOffsets[3];
    granny_int32 KnotControlCount;
    granny_uint8 * KnotsControls;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_curve_data_d3_k8u_c8u) == (sizeof(granny_curve_data_header) + 
                                                             sizeof(granny_uint16) + 
                                                             sizeof(granny_real32) * 3 + 
                                                             sizeof(granny_real32) * 3 + 
                                                             sizeof(granny_int32) + 
                                                             sizeof(granny_uint8 *)));

struct granny_curve_data_d4n_k16u_c15u
{
    granny_curve_data_header CurveDataHeader;
    granny_uint16 ScaleOffsetTableEntries;
    granny_real32 OneOverKnotScale;
    granny_int32 KnotControlCount;
    granny_uint16 * KnotsControls;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_curve_data_d4n_k16u_c15u) == (sizeof(granny_curve_data_header) + 
                                                                sizeof(granny_uint16) + 
                                                                sizeof(granny_real32) + 
                                                                sizeof(granny_int32) + 
                                                                sizeof(granny_uint16 *)));

struct granny_curve_data_d4n_k8u_c7u
{
    granny_curve_data_header CurveDataHeader;
    granny_uint16 ScaleOffsetTableEntries;
    granny_real32 OneOverKnotScale;
    granny_int32 KnotControlCount;
    granny_uint8 * KnotsControls;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_curve_data_d4n_k8u_c7u) == (sizeof(granny_curve_data_header) + 
                                                              sizeof(granny_uint16) + 
                                                              sizeof(granny_real32) + 
                                                              sizeof(granny_int32) + 
                                                              sizeof(granny_uint8 *)));

struct granny_curve_data_da_identity
{
    granny_curve_data_header CurveDataHeader;
    granny_int16 Dimension;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_curve_data_da_identity) == (sizeof(granny_curve_data_header) + 
                                                              sizeof(granny_int16)));

struct granny_curve_data_da_constant32f
{
    granny_curve_data_header CurveDataHeader;
    granny_int16 Padding;
    granny_int32 ControlCount;
    granny_real32 * Controls;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_curve_data_da_constant32f) == (sizeof(granny_curve_data_header) + 
                                                                 sizeof(granny_int16) + 
                                                                 sizeof(granny_int32) + 
                                                                 sizeof(granny_real32 *)));

struct granny_curve_data_d3_constant32f
{
    granny_curve_data_header CurveDataHeader;
    granny_int16 Padding;
    granny_real32 Controls[3];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_curve_data_d3_constant32f) == (sizeof(granny_curve_data_header) + 
                                                                 sizeof(granny_int16) + 
                                                                 sizeof(granny_real32) * 3));

struct granny_curve_data_d4_constant32f
{
    granny_curve_data_header CurveDataHeader;
    granny_int16 Padding;
    granny_real32 Controls[4];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_curve_data_d4_constant32f) == (sizeof(granny_curve_data_header) + 
                                                                 sizeof(granny_int16) + 
                                                                 sizeof(granny_real32) * 4));

struct granny_curve_data_d9i1_k16u_c16u
{
    granny_curve_data_header CurveDataHeader;
    granny_uint16 OneOverKnotScaleTrunc;
    granny_real32 ControlScale;
    granny_real32 ControlOffset;
    granny_int32 KnotControlCount;
    granny_uint16 * KnotsControls;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_curve_data_d9i1_k16u_c16u) == (sizeof(granny_curve_data_header) + 
                                                                 sizeof(granny_uint16) + 
                                                                 sizeof(granny_real32) + 
                                                                 sizeof(granny_real32) + 
                                                                 sizeof(granny_int32) + 
                                                                 sizeof(granny_uint16 *)));

struct granny_curve_data_d9i3_k16u_c16u
{
    granny_curve_data_header CurveDataHeader;
    granny_uint16 OneOverKnotScaleTrunc;
    granny_real32 ControlScales[3];
    granny_real32 ControlOffsets[3];
    granny_int32 KnotControlCount;
    granny_uint16 * KnotsControls;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_curve_data_d9i3_k16u_c16u) == (sizeof(granny_curve_data_header) + 
                                                                 sizeof(granny_uint16) + 
                                                                 sizeof(granny_real32) * 3 + 
                                                                 sizeof(granny_real32) * 3 + 
                                                                 sizeof(granny_int32) + 
                                                                 sizeof(granny_uint16 *)));

struct granny_curve_data_d9i1_k8u_c8u
{
    granny_curve_data_header CurveDataHeader;
    granny_uint16 OneOverKnotScaleTrunc;
    granny_real32 ControlScale;
    granny_real32 ControlOffset;
    granny_int32 KnotControlCount;
    granny_uint8 * KnotsControls;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_curve_data_d9i1_k8u_c8u) == (sizeof(granny_curve_data_header) + 
                                                               sizeof(granny_uint16) + 
                                                               sizeof(granny_real32) + 
                                                               sizeof(granny_real32) + 
                                                               sizeof(granny_int32) + 
                                                               sizeof(granny_uint8 *)));

struct granny_curve_data_d9i3_k8u_c8u
{
    granny_curve_data_header CurveDataHeader;
    granny_uint16 OneOverKnotScaleTrunc;
    granny_real32 ControlScales[3];
    granny_real32 ControlOffsets[3];
    granny_int32 KnotControlCount;
    granny_uint8 * KnotsControls;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_curve_data_d9i3_k8u_c8u) == (sizeof(granny_curve_data_header) + 
                                                               sizeof(granny_uint16) + 
                                                               sizeof(granny_real32) * 3 + 
                                                               sizeof(granny_real32) * 3 + 
                                                               sizeof(granny_int32) + 
                                                               sizeof(granny_uint8 *)));

struct granny_curve_data_d3i1_k32f_c32f
{
    granny_curve_data_header CurveDataHeader;
    granny_int16 Padding;
    granny_real32 ControlScales[3];
    granny_real32 ControlOffsets[3];
    granny_int32 KnotControlCount;
    granny_real32 * KnotsControls;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_curve_data_d3i1_k32f_c32f) == (sizeof(granny_curve_data_header) + 
                                                                 sizeof(granny_int16) + 
                                                                 sizeof(granny_real32) * 3 + 
                                                                 sizeof(granny_real32) * 3 + 
                                                                 sizeof(granny_int32) + 
                                                                 sizeof(granny_real32 *)));

struct granny_curve_data_d3i1_k16u_c16u
{
    granny_curve_data_header CurveDataHeader;
    granny_uint16 OneOverKnotScaleTrunc;
    granny_real32 ControlScales[3];
    granny_real32 ControlOffsets[3];
    granny_int32 KnotControlCount;
    granny_uint16 * KnotsControls;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_curve_data_d3i1_k16u_c16u) == (sizeof(granny_curve_data_header) + 
                                                                 sizeof(granny_uint16) + 
                                                                 sizeof(granny_real32) * 3 + 
                                                                 sizeof(granny_real32) * 3 + 
                                                                 sizeof(granny_int32) + 
                                                                 sizeof(granny_uint16 *)));

struct granny_curve_data_d3i1_k8u_c8u
{
    granny_curve_data_header CurveDataHeader;
    granny_uint16 OneOverKnotScaleTrunc;
    granny_real32 ControlScales[3];
    granny_real32 ControlOffsets[3];
    granny_int32 KnotControlCount;
    granny_uint8 * KnotsControls;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_curve_data_d3i1_k8u_c8u) == (sizeof(granny_curve_data_header) + 
                                                               sizeof(granny_uint16) + 
                                                               sizeof(granny_real32) * 3 + 
                                                               sizeof(granny_real32) * 3 + 
                                                               sizeof(granny_int32) + 
                                                               sizeof(granny_uint8 *)));

struct granny_curve_builder;

struct granny_material_map
{
    char const * Usage;
    granny_material * Material;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_material_map) == (sizeof(char const *) + 
                                                    sizeof(granny_material *)));

struct granny_material
{
    char const * Name;
    granny_int32 MapCount;
    granny_material_map * Maps;
    granny_texture * Texture;
    granny_variant ExtendedData;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_material) == (sizeof(char const *) + 
                                                sizeof(granny_int32) + 
                                                sizeof(granny_material_map *) + 
                                                sizeof(granny_texture *) + 
                                                sizeof(granny_variant)));

struct granny_pixel_layout
{
    granny_int32 BytesPerPixel;
    granny_int32 ShiftForComponent[4];
    granny_int32 BitsForComponent[4];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pixel_layout) == (sizeof(granny_int32) + 
                                                    sizeof(granny_int32) * 4 + 
                                                    sizeof(granny_int32) * 4));

struct granny_animation
{
    char const * Name;
    granny_real32 Duration;
    granny_real32 TimeStep;
    granny_real32 Oversampling;
    granny_int32 TrackGroupCount;
    granny_track_group ** TrackGroups;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_animation) == (sizeof(char const *) + 
                                                 sizeof(granny_real32) + 
                                                 sizeof(granny_real32) + 
                                                 sizeof(granny_real32) + 
                                                 sizeof(granny_int32) + 
                                                 sizeof(granny_track_group **)));

struct granny_bound_transform_track
{
    granny_int16 SourceTrackIndex;
    granny_int8 QuaternionMode;
    granny_int8 Flags;
    granny_transform_track const * SourceTrack;
    granny_track_sampler * Sampler;
    granny_real32 LODError;
    granny_transform LODTransform;
};

struct granny_animation_binding_identifier
{
    granny_animation const * Animation;
    granny_int32x SourceTrackGroupIndex;
    char const * TrackPattern;
    char const * BonePattern;
    granny_model const * OnModel;
    granny_model * FromBasis;
    granny_model * ToBasis;
};

struct granny_animation_binding
{
    granny_animation_binding_identifier ID;
    void * RebasingMemory;
    granny_int32x TrackBindingCount;
    granny_bound_transform_track * TrackBindings;
    granny_animation_binding * Left;
    granny_animation_binding * Right;
    granny_int32x UsedBy;
    granny_animation_binding * PreviousUnused;
    granny_animation_binding * NextUnused;
};

struct granny_animation_lod_builder;

struct granny_art_tool_info
{
    char const * FromArtToolName;
    granny_int32 ArtToolMajorRevision;
    granny_int32 ArtToolMinorRevision;
    granny_real32 UnitsPerMeter;
    granny_triple Origin;
    granny_triple RightVector;
    granny_triple UpVector;
    granny_triple BackVector;
    granny_variant ExtendedData;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_art_tool_info) == (sizeof(char const *) + 
                                                     sizeof(granny_int32) + 
                                                     sizeof(granny_int32) + 
                                                     sizeof(granny_real32) + 
                                                     sizeof(granny_triple) + 
                                                     sizeof(granny_triple) + 
                                                     sizeof(granny_triple) + 
                                                     sizeof(granny_triple) + 
                                                     sizeof(granny_variant)));

struct granny_bspline_solver;

struct granny_bspline_error
{
    granny_int32x MaximumSquaredErrorKnotIndex;
    granny_int32x MaximumSquaredErrorSampleIndex;
    granny_real32 MaximumSquaredError;
    granny_real32 AccumulatedSquaredError;
};

struct granny_camera
{
    granny_real32 WpOverHp;
    granny_real32 WrOverHr;
    granny_real32 WwOverHw;
    granny_real32 FOVY;
    granny_real32 NearClipPlane;
    granny_real32 FarClipPlane;
    granny_real32 ZRangeEpsilon;
    granny_camera_output_z_range OutputZRange;
    bool UseQuaternionOrientation;
    granny_quad Orientation;
    granny_matrix_3x3 OrientationMatrix;
    granny_triple Position;
    granny_triple ElevAzimRoll;
    granny_triple Offset;
    granny_matrix_4x4 View4x4;
    granny_matrix_4x4 InverseView4x4;
    granny_matrix_4x4 Projection4x4;
    granny_matrix_4x4 InverseProjection4x4;
};

struct granny_compress_curve_parameters
{
    granny_int32x DesiredDegree;
    bool AllowDegreeReduction;
    bool AllowReductionOnMissedTolerance;
    granny_real32 ErrorTolerance;
    granny_real32 C0Threshold;
    granny_real32 C1Threshold;
    granny_data_type_definition ** PossibleCompressionTypes;
    granny_int32x PossibleCompressionTypesCount;
    granny_data_type_definition * ConstantCompressionType;
    granny_data_type_definition * IdentityCompressionType;
    granny_real32 const * IdentityVector;
};

struct granny_control;

struct granny_controlled_animation_builder;

struct granny_blend_dag_node;

struct granny_dag_pose_cache;

struct granny_file_data_tree_writer;

struct granny_exporter_info
{
    char * ExporterName;
    granny_int32 ExporterMajorRevision;
    granny_int32 ExporterMinorRevision;
    granny_int32 ExporterCustomization;
    granny_int32 ExporterBuildNumber;
    granny_variant ExtendedData;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_exporter_info) == (sizeof(char *) + 
                                                     sizeof(granny_int32) + 
                                                     sizeof(granny_int32) + 
                                                     sizeof(granny_int32) + 
                                                     sizeof(granny_int32) + 
                                                     sizeof(granny_variant)));

struct granny_file
{
    bool IsByteReversed;
    granny_grn_file_header * Header;
    granny_grn_file_magic_value *  SourceMagicValue;
    granny_int32x SectionCount;
    void ** Sections;
    bool * Marshalled;
    bool * IsUserMemory;
    void * ConversionBuffer;
};

struct granny_file_location
{
    granny_uint32 SectionIndex;
    granny_uint32 BufferIndex;
    granny_uint32 Offset;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_file_location) == (sizeof(granny_uint32) + 
                                                     sizeof(granny_uint32) + 
                                                     sizeof(granny_uint32)));

struct granny_file_fixup;

struct granny_file_builder;

struct granny_file_compressor;

struct granny_grn_file_magic_value
{
    granny_uint32 MagicValue[4];
    granny_uint32 HeaderSize;
    granny_uint32 HeaderFormat;
    granny_uint32 Reserved[2];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_grn_file_magic_value) == (sizeof(granny_uint32) * 4 + 
                                                            sizeof(granny_uint32) + 
                                                            sizeof(granny_uint32) + 
                                                            sizeof(granny_uint32) * 2));

struct granny_grn_reference
{
    granny_uint32 SectionIndex;
    granny_uint32 Offset;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_grn_reference) == (sizeof(granny_uint32) + 
                                                     sizeof(granny_uint32)));

struct granny_grn_pointer_fixup
{
    granny_uint32 FromOffset;
    granny_grn_reference To;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_grn_pointer_fixup) == (sizeof(granny_uint32) + 
                                                         sizeof(granny_grn_reference)));

struct granny_grn_mixed_marshalling_fixup
{
    granny_uint32 Count;
    granny_uint32 Offset;
    granny_grn_reference Type;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_grn_mixed_marshalling_fixup) == (sizeof(granny_uint32) + 
                                                                   sizeof(granny_uint32) + 
                                                                   sizeof(granny_grn_reference)));

struct granny_grn_section
{
    granny_uint32 Format;
    granny_uint32 DataOffset;
    granny_uint32 DataSize;
    granny_uint32 ExpandedDataSize;
    granny_uint32 InternalAlignment;
    granny_uint32 First16Bit;
    granny_uint32 First8Bit;
    granny_uint32 PointerFixupArrayOffset;
    granny_uint32 PointerFixupArrayCount;
    granny_uint32 MixedMarshallingFixupArrayOffset;
    granny_uint32 MixedMarshallingFixupArrayCount;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_grn_section) == (sizeof(granny_uint32) + 
                                                   sizeof(granny_uint32) + 
                                                   sizeof(granny_uint32) + 
                                                   sizeof(granny_uint32) + 
                                                   sizeof(granny_uint32) + 
                                                   sizeof(granny_uint32) + 
                                                   sizeof(granny_uint32) + 
                                                   sizeof(granny_uint32) + 
                                                   sizeof(granny_uint32) + 
                                                   sizeof(granny_uint32) + 
                                                   sizeof(granny_uint32)));

struct granny_grn_file_header
{
    granny_uint32 Version;
    granny_uint32 TotalSize;
    granny_uint32 CRC;
    granny_uint32 SectionArrayOffset;
    granny_uint32 SectionArrayCount;
    granny_grn_reference RootObjectTypeDefinition;
    granny_grn_reference RootObject;
    granny_uint32 TypeTag;
    granny_uint32 ExtraTags[GrannyGRNExtraTagCount];
    granny_uint32 StringDatabaseCRC;
    granny_uint32 ReservedUnused[3];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_grn_file_header) == (sizeof(granny_uint32) + 
                                                       sizeof(granny_uint32) + 
                                                       sizeof(granny_uint32) + 
                                                       sizeof(granny_uint32) + 
                                                       sizeof(granny_uint32) + 
                                                       sizeof(granny_grn_reference) + 
                                                       sizeof(granny_grn_reference) + 
                                                       sizeof(granny_uint32) + 
                                                       sizeof(granny_uint32) * GrannyGRNExtraTagCount + 
                                                       sizeof(granny_uint32) + 
                                                       sizeof(granny_uint32) * 3));

struct granny_file_info
{
    granny_art_tool_info * ArtToolInfo;
    granny_exporter_info * ExporterInfo;
    char const * FromFileName;
    granny_int32 TextureCount;
    granny_texture ** Textures;
    granny_int32 MaterialCount;
    granny_material ** Materials;
    granny_int32 SkeletonCount;
    granny_skeleton ** Skeletons;
    granny_int32 VertexDataCount;
    granny_vertex_data ** VertexDatas;
    granny_int32 TriTopologyCount;
    granny_tri_topology ** TriTopologies;
    granny_int32 MeshCount;
    granny_mesh ** Meshes;
    granny_int32 ModelCount;
    granny_model ** Models;
    granny_int32 TrackGroupCount;
    granny_track_group ** TrackGroups;
    granny_int32 AnimationCount;
    granny_animation ** Animations;
    granny_variant ExtendedData;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_file_info) == (sizeof(granny_art_tool_info *) + 
                                                 sizeof(granny_exporter_info *) + 
                                                 sizeof(char const *) + 
                                                 sizeof(granny_int32) + 
                                                 sizeof(granny_texture **) + 
                                                 sizeof(granny_int32) + 
                                                 sizeof(granny_material **) + 
                                                 sizeof(granny_int32) + 
                                                 sizeof(granny_skeleton **) + 
                                                 sizeof(granny_int32) + 
                                                 sizeof(granny_vertex_data **) + 
                                                 sizeof(granny_int32) + 
                                                 sizeof(granny_tri_topology **) + 
                                                 sizeof(granny_int32) + 
                                                 sizeof(granny_mesh **) + 
                                                 sizeof(granny_int32) + 
                                                 sizeof(granny_model **) + 
                                                 sizeof(granny_int32) + 
                                                 sizeof(granny_track_group **) + 
                                                 sizeof(granny_int32) + 
                                                 sizeof(granny_animation **) + 
                                                 sizeof(granny_variant)));

struct granny_file_reader
{
    char const * SourceFileName;
    granny_int32x SourceLineNumber;
    granny_close_file_reader_callback * CloseFileReaderCallback;
    granny_read_at_most_callback * ReadAtMostCallback;
};

struct granny_file_writer
{
    char const * SourceFileName;
    granny_int32x SourceLineNumber;
    granny_delete_file_writer_callback * DeleteFileWriterCallback;
    granny_seek_file_writer_callback * SeekWriterCallback;
    granny_write_file_writer_callback * WriteCallback;
    granny_begincrc_file_writer_callback * BeginCRCCallback;
    granny_endcrc_file_writer_callback * EndCRCCallback;
    bool CRCing;
    granny_uint32 CRC;
};

struct granny_fixed_allocator_unit
{
    granny_fixed_allocator_unit * Next;
};

struct granny_fixed_allocator_block
{
    granny_int32x UnitCount;
    granny_uint8 * Units;
    granny_fixed_allocator_unit * FirstFreeUnit;
    granny_fixed_allocator_block * Next;
    granny_fixed_allocator_block * Previous;
};

struct granny_fixed_allocator
{
    granny_int32x UnitSize;
    granny_int32x UnitsPerBlock;
    granny_fixed_allocator_block Sentinel;
};

struct granny_box_intersection
{
    granny_real32 MinT;
    granny_triple MinNormal;
    granny_real32 MaxT;
    granny_triple MaxNormal;
};

struct granny_triangle_intersection
{
    granny_real32 T;
    granny_real32 TriangleU;
    granny_real32 TriangleV;
    granny_triple EdgeU;
    granny_triple EdgeV;
    bool Backfacing;
};

struct granny_local_pose;

struct granny_log_callback
{
    granny_log_function * Function;
    void * UserData;
};

struct granny_allocation_header;

struct granny_allocation_information
{
    void * Memory;
    granny_intaddrx RequestedSize;
    granny_intaddrx ActualSize;
    char const * SourceFileName;
    granny_int32x SourceLineNumber;
};

struct granny_memory_arena;

struct granny_bone_binding
{
    char const * BoneName;
    granny_triple OBBMin;
    granny_triple OBBMax;
    granny_int32 TriangleCount;
    granny_int32 * TriangleIndices;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_bone_binding) == (sizeof(char const *) + 
                                                    sizeof(granny_triple) + 
                                                    sizeof(granny_triple) + 
                                                    sizeof(granny_int32) + 
                                                    sizeof(granny_int32 *)));

struct granny_material_binding
{
    granny_material * Material;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_material_binding) == (sizeof(granny_material *)));

struct granny_morph_target
{
    char const * ScalarName;
    granny_vertex_data * VertexData;
    granny_int32 DataIsDeltas;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_morph_target) == (sizeof(char const *) + 
                                                    sizeof(granny_vertex_data *) + 
                                                    sizeof(granny_int32)));

struct granny_mesh
{
    char const * Name;
    granny_vertex_data * PrimaryVertexData;
    granny_int32 MorphTargetCount;
    granny_morph_target * MorphTargets;
    granny_tri_topology * PrimaryTopology;
    granny_int32 MaterialBindingCount;
    granny_material_binding * MaterialBindings;
    granny_int32 BoneBindingCount;
    granny_bone_binding * BoneBindings;
    granny_variant ExtendedData;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_mesh) == (sizeof(char const *) + 
                                            sizeof(granny_vertex_data *) + 
                                            sizeof(granny_int32) + 
                                            sizeof(granny_morph_target *) + 
                                            sizeof(granny_tri_topology *) + 
                                            sizeof(granny_int32) + 
                                            sizeof(granny_material_binding *) + 
                                            sizeof(granny_int32) + 
                                            sizeof(granny_bone_binding *) + 
                                            sizeof(granny_variant)));

struct granny_mesh_binding;

struct granny_mesh_builder;

struct granny_mesh_deformer;

struct granny_oodle1_state;

struct granny_periodic_loop
{
    granny_real32 Radius;
    granny_real32 dAngle;
    granny_real32 dZ;
    granny_triple BasisX;
    granny_triple BasisY;
    granny_triple Axis;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_periodic_loop) == (sizeof(granny_real32) + 
                                                     sizeof(granny_real32) + 
                                                     sizeof(granny_real32) + 
                                                     sizeof(granny_triple) + 
                                                     sizeof(granny_triple) + 
                                                     sizeof(granny_triple)));

struct granny_pointer_hash;

struct granny_bone
{
    char const * Name;
    granny_int32 ParentIndex;
    granny_transform LocalTransform;
    granny_matrix_4x4 InverseWorld4x4;
    granny_real32 LODError;
    granny_variant ExtendedData;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_bone) == (sizeof(char const *) + 
                                            sizeof(granny_int32) + 
                                            sizeof(granny_transform) + 
                                            sizeof(granny_matrix_4x4) + 
                                            sizeof(granny_real32) + 
                                            sizeof(granny_variant)));

struct granny_skeleton
{
    char const * Name;
    granny_int32 BoneCount;
    granny_bone * Bones;
    granny_int32 LODType;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_skeleton) == (sizeof(char const *) + 
                                                sizeof(granny_int32) + 
                                                sizeof(granny_bone *) + 
                                                sizeof(granny_int32)));

struct granny_skeleton_builder;

struct granny_spu_animation
{
    char const * Name;
    granny_real32 Duration;
    granny_real32 TimeStep;
    granny_real32 Oversampling;
    granny_int32 TrackGroupCount;
    granny_spu_track_group **  TrackGroups;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_spu_animation) == (sizeof(char const *) + 
                                                     sizeof(granny_real32) + 
                                                     sizeof(granny_real32) + 
                                                     sizeof(granny_real32) + 
                                                     sizeof(granny_int32) + 
                                                     sizeof(granny_spu_track_group ** )));

struct granny_spu_animation_binding_id
{
    granny_spu_animation const*    Animation;
    granny_spu_track_group const*  TrackGroup;
    granny_model const*            Model;
    char const * TrackPattern;
    char const * BonePattern;
    granny_int32 TransformTrackCount;
    granny_spu_transform_track *  TransformTracks;
    granny_int32 CurveByteCount;
    granny_uint8 *                CurveBytes;
};

struct granny_spu_animation_binding
{
    granny_spu_animation_binding_id ID;
    granny_int32 TrackNameRemapCount;
    granny_int32 *  TrackNameRemaps;
    granny_spu_animation_binding * Left;
    granny_spu_animation_binding * Right;
    granny_int32 UsedBy;
    granny_spu_animation_binding * PreviousUnused;
    granny_spu_animation_binding * NextUnused;
};

struct granny_spu_animation_info
{
    granny_int32 SPUAnimationCount;
    granny_spu_animation **  SPUAnimations;
    granny_variant ExtendedData;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_spu_animation_info) == (sizeof(granny_int32) + 
                                                          sizeof(granny_spu_animation ** ) + 
                                                          sizeof(granny_variant)));

struct granny_controlled_spu_animation_builder;

struct granny_spu_curve_header_basic
{
    granny_uint16 TotalSize;
    granny_uint16 CurveType;
    granny_uint8 Degree;
    granny_uint8 Dimension;
    granny_uint16 ReplicationType;
    granny_uint16 KnotCount;
    granny_uint16 KnotStart;
    granny_uint16 ControlStart;
    granny_uint16 ControlStride;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_spu_curve_header_basic) == (sizeof(granny_uint16) + 
                                                              sizeof(granny_uint16) + 
                                                              sizeof(granny_uint8) + 
                                                              sizeof(granny_uint8) + 
                                                              sizeof(granny_uint16) + 
                                                              sizeof(granny_uint16) + 
                                                              sizeof(granny_uint16) + 
                                                              sizeof(granny_uint16) + 
                                                              sizeof(granny_uint16)));

struct granny_spu_curve_header_quantized
{
    granny_spu_curve_header_basic BasicHeader;
    granny_real32 OneOverKnotScale;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_spu_curve_header_quantized) == (sizeof(granny_spu_curve_header_basic) + 
                                                                  sizeof(granny_real32)));

struct granny_spu_curve_header_quantized4n
{
    granny_spu_curve_header_basic BasicHeader;
    granny_int32 ScaleOffsetTableEntries;
    granny_real32 OneOverKnotScale;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_spu_curve_header_quantized4n) == (sizeof(granny_spu_curve_header_basic) + 
                                                                    sizeof(granny_int32) + 
                                                                    sizeof(granny_real32)));

struct granny_spu_transform_track
{
    granny_uint32 FromNameIndex;
    granny_uint32 Flags;
    granny_real32 AnimLODValue;
    granny_transform LODTransform;
    granny_uint32 PosCurveOffset;
    granny_uint32 OriCurveOffset;
    granny_uint32 SSCurveOffset;
    granny_uint32 Reserved;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_spu_transform_track) == (sizeof(granny_uint32) + 
                                                           sizeof(granny_uint32) + 
                                                           sizeof(granny_real32) + 
                                                           sizeof(granny_transform) + 
                                                           sizeof(granny_uint32) + 
                                                           sizeof(granny_uint32) + 
                                                           sizeof(granny_uint32) + 
                                                           sizeof(granny_uint32)));

struct granny_spu_track_group
{
    char const * Name;
    granny_int32 Flags;
    granny_int32 TrackNameCount;
    char const**  TrackNames;
    granny_int32 TransformTrackCount;
    granny_spu_transform_track *  TransformTracks;
    granny_int32 CurveByteCount;
    granny_uint8 *  CurveBytes;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_spu_track_group) == (sizeof(char const *) + 
                                                       sizeof(granny_int32) + 
                                                       sizeof(granny_int32) + 
                                                       sizeof(char const** ) + 
                                                       sizeof(granny_int32) + 
                                                       sizeof(granny_spu_transform_track * ) + 
                                                       sizeof(granny_int32) + 
                                                       sizeof(granny_uint8 * )));

struct granny_allocated_block;

struct granny_stack_allocator
{
    granny_int32x UnitSize;
    granny_int32x UnitsPerBlock;
    granny_int32x TotalUsedUnitCount;
    granny_allocated_block * LastBlock;
    granny_int32x MaxUnits;
    granny_int32x ActiveBlocks;
    granny_int32x MaxActiveBlocks;
    granny_allocated_block ** BlockDirectory;
};

struct granny_counter_results
{
    char const * Name;
    granny_int32x Count;
    granny_real64x TotalSeconds;
    granny_real64x SecondsWithoutChildren;
    granny_real64x TotalCycles;
    granny_real64x TotalCyclesWithoutChildren;
    granny_int32x PeakCount;
    granny_real64x PeakTotalSeconds;
    granny_real64x PeakSecondsWithoutChildren;
};

struct granny_animation_binding_cache_status
{
    granny_int32x TotalBindingsCreated;
    granny_int32x TotalBindingsDestroyed;
    granny_int32x DirectAcquireCount;
    granny_int32x IndirectAcquireCount;
    granny_int32x ReleaseCount;
    granny_int32x CurrentTotalBindingCount;
    granny_int32x CurrentUsedBindingCount;
    granny_int32x CacheHits;
    granny_int32x CacheMisses;
    granny_int32x ExplicitFlushCount;
    granny_int32x ExplicitFlushFrees;
    granny_int32x OverflowFrees;
};

struct granny_stat_hud_alloc_point
{
    char * SourceFilename;
    granny_int32x SourceLineNumber;
    granny_intaddrx AllocationCount;
    granny_intaddrx BytesRequested;
    granny_intaddrx BytesAllocated;
};

struct granny_stat_hud_perf_point
{
    char * Name;
    granny_int32x Count;
    granny_real64x TotalSeconds;
    granny_real64x TotalCycles;
};

struct granny_stat_hud_model_instances
{
    granny_int32x TotalInstanceCount;
    granny_int32x TotalInstancedBoneCount;
    granny_int32x TotalUsedModelCount;
    granny_int32x MaxInstanceCount;
    granny_int32x MaxBoneCount;
    granny_int32x MaxInstanceControlCount;
};

struct granny_stat_hud_model_controls
{
    granny_int32x TotalControlCount;
    granny_int32x ActiveControlCount;
    granny_int32x ActiveAndWeightedControlCount;
    granny_int32x ActiveAndWeightedButEasedOutControlCount;
    granny_int32x CompletableControlCount;
    granny_int32x CompletedControlCount;
    granny_int32x UnusedControlCount;
    granny_real32 MinClockTime;
    granny_real32 MaxClockTime;
    granny_real32 MinCompletionTime;
    granny_real32 MaxCompletionTime;
};

struct granny_stat_hud_footprint
{
    granny_intaddrx TotalAllocationCount;
    granny_intaddrx TotalBytesRequested;
    granny_intaddrx TotalBytesAllocated;
};

struct granny_stat_hud_perf
{
    granny_real64x TotalSeconds;
    granny_real64x TotalCycles;
};

struct granny_stat_hud_animation_types
{
    granny_int32x TotalTrackCount;
    granny_int32x TrackTypes[3][3][3][2];
};

struct granny_stat_hud
{
    granny_int32x FrameCount;
    granny_stat_hud_model_instances ModelInstances;
    granny_stat_hud_model_controls Controls;
    granny_animation_binding_cache_status AnimBindingCacheStatus;
    granny_stat_hud_animation_types AnimTypes;
    granny_stat_hud_footprint Footprint;
    granny_int32x AllocPointCount;
    granny_stat_hud_alloc_point * AllocPoints;
    granny_stat_hud_perf Perf;
    granny_int32x PerfPointCount;
    granny_stat_hud_perf_point * PerfPoints;
};

struct granny_string_database
{
    granny_int32 StringCount;
    char ** Strings;
    granny_uint32 DatabaseCRC;
    granny_variant ExtendedData;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_string_database) == (sizeof(granny_int32) + 
                                                       sizeof(char **) + 
                                                       sizeof(granny_uint32) + 
                                                       sizeof(granny_variant)));

struct granny_string_table;

struct granny_system_clock
{
    granny_uint32 Data[4];
};

struct granny_tangent_frame
{
    granny_triple U;
    granny_triple V;
    granny_triple N;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_tangent_frame) == (sizeof(granny_triple) + 
                                                     sizeof(granny_triple) + 
                                                     sizeof(granny_triple)));

struct granny_texture_mip_level
{
    granny_int32 Stride;
    granny_int32 PixelByteCount;
    void * PixelBytes;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_texture_mip_level) == (sizeof(granny_int32) + 
                                                         sizeof(granny_int32) + 
                                                         sizeof(void *)));

struct granny_texture_image
{
    granny_int32 MIPLevelCount;
    granny_texture_mip_level * MIPLevels;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_texture_image) == (sizeof(granny_int32) + 
                                                     sizeof(granny_texture_mip_level *)));

struct granny_texture
{
    char const * FromFileName;
    granny_int32 TextureType;
    granny_int32 Width;
    granny_int32 Height;
    granny_int32 Encoding;
    granny_int32 SubFormat;
    granny_pixel_layout Layout;
    granny_int32 ImageCount;
    granny_texture_image * Images;
    granny_variant ExtendedData;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_texture) == (sizeof(char const *) + 
                                               sizeof(granny_int32) + 
                                               sizeof(granny_int32) + 
                                               sizeof(granny_int32) + 
                                               sizeof(granny_int32) + 
                                               sizeof(granny_int32) + 
                                               sizeof(granny_pixel_layout) + 
                                               sizeof(granny_int32) + 
                                               sizeof(granny_texture_image *) + 
                                               sizeof(granny_variant)));

struct granny_texture_builder;

struct granny_vector_track
{
    char const * Name;
    granny_uint32 TrackKey;
    granny_int32 Dimension;
    granny_curve2 ValueCurve;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_vector_track) == (sizeof(char const *) + 
                                                    sizeof(granny_uint32) + 
                                                    sizeof(granny_int32) + 
                                                    sizeof(granny_curve2)));

struct granny_transform_track
{
    char const * Name;
    granny_int32 Flags;
    granny_curve2 OrientationCurve;
    granny_curve2 PositionCurve;
    granny_curve2 ScaleShearCurve;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_transform_track) == (sizeof(char const *) + 
                                                       sizeof(granny_int32) + 
                                                       sizeof(granny_curve2) + 
                                                       sizeof(granny_curve2) + 
                                                       sizeof(granny_curve2)));

struct granny_text_track_entry
{
    granny_real32 TimeStamp;
    char const * Text;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_text_track_entry) == (sizeof(granny_real32) + 
                                                        sizeof(char const *)));

struct granny_text_track
{
    char const * Name;
    granny_int32 EntryCount;
    granny_text_track_entry * Entries;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_text_track) == (sizeof(char const *) + 
                                                  sizeof(granny_int32) + 
                                                  sizeof(granny_text_track_entry *)));

struct granny_track_group
{
    char const * Name;
    granny_int32 VectorTrackCount;
    granny_vector_track * VectorTracks;
    granny_int32 TransformTrackCount;
    granny_transform_track * TransformTracks;
    granny_int32 TransformLODErrorCount;
    granny_real32 * TransformLODErrors;
    granny_int32 TextTrackCount;
    granny_text_track * TextTracks;
    granny_transform InitialPlacement;
    granny_int32 Flags;
    granny_triple LoopTranslation;
    granny_periodic_loop * PeriodicLoop;
    granny_transform_track * RootMotion;
    granny_variant ExtendedData;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_track_group) == (sizeof(char const *) + 
                                                   sizeof(granny_int32) + 
                                                   sizeof(granny_vector_track *) + 
                                                   sizeof(granny_int32) + 
                                                   sizeof(granny_transform_track *) + 
                                                   sizeof(granny_int32) + 
                                                   sizeof(granny_real32 *) + 
                                                   sizeof(granny_int32) + 
                                                   sizeof(granny_text_track *) + 
                                                   sizeof(granny_transform) + 
                                                   sizeof(granny_int32) + 
                                                   sizeof(granny_triple) + 
                                                   sizeof(granny_periodic_loop *) + 
                                                   sizeof(granny_transform_track *) + 
                                                   sizeof(granny_variant)));

struct granny_track_group_builder;

struct granny_track_group_sampler;

struct granny_track_mask;

struct granny_unbound_weight
{
    char const*  Name;
    granny_real32 Weight;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_unbound_weight) == (sizeof(char const* ) + 
                                                      sizeof(granny_real32)));

struct granny_unbound_track_mask
{
    granny_real32 DefaultWeight;
    granny_int32 WeightCount;
    granny_unbound_weight *  Weights;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_unbound_track_mask) == (sizeof(granny_real32) + 
                                                          sizeof(granny_int32) + 
                                                          sizeof(granny_unbound_weight * )));

struct granny_sample_context
{
    granny_real32 LocalClock;
    granny_real32 LocalDuration;
    bool UnderflowLoop;
    bool OverflowLoop;
    granny_int32x FrameIndex;
};

struct granny_tri_material_group
{
    granny_int32 MaterialIndex;
    granny_int32 TriFirst;
    granny_int32 TriCount;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_tri_material_group) == (sizeof(granny_int32) + 
                                                          sizeof(granny_int32) + 
                                                          sizeof(granny_int32)));

struct granny_tri_annotation_set
{
    char const * Name;
    granny_data_type_definition * TriAnnotationType;
    granny_int32 TriAnnotationCount;
    granny_uint8 * TriAnnotations;
    granny_int32 IndicesMapFromTriToAnnotation;
    granny_int32 TriAnnotationIndexCount;
    granny_int32 * TriAnnotationIndices;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_tri_annotation_set) == (sizeof(char const *) + 
                                                          sizeof(granny_data_type_definition *) + 
                                                          sizeof(granny_int32) + 
                                                          sizeof(granny_uint8 *) + 
                                                          sizeof(granny_int32) + 
                                                          sizeof(granny_int32) + 
                                                          sizeof(granny_int32 *)));

struct granny_tri_topology
{
    granny_int32 GroupCount;
    granny_tri_material_group * Groups;
    granny_int32 IndexCount;
    granny_int32 * Indices;
    granny_int32 Index16Count;
    granny_uint16 * Indices16;
    granny_int32 VertexToVertexCount;
    granny_int32 * VertexToVertexMap;
    granny_int32 VertexToTriangleCount;
    granny_int32 * VertexToTriangleMap;
    granny_int32 SideToNeighborCount;
    granny_uint32 * SideToNeighborMap;
    granny_int32 BonesForTriangleCount;
    granny_int32 * BonesForTriangle;
    granny_int32 TriangleToBoneCount;
    granny_int32 * TriangleToBoneIndices;
    granny_int32 TriAnnotationSetCount;
    granny_tri_annotation_set * TriAnnotationSets;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_tri_topology) == (sizeof(granny_int32) + 
                                                    sizeof(granny_tri_material_group *) + 
                                                    sizeof(granny_int32) + 
                                                    sizeof(granny_int32 *) + 
                                                    sizeof(granny_int32) + 
                                                    sizeof(granny_uint16 *) + 
                                                    sizeof(granny_int32) + 
                                                    sizeof(granny_int32 *) + 
                                                    sizeof(granny_int32) + 
                                                    sizeof(granny_int32 *) + 
                                                    sizeof(granny_int32) + 
                                                    sizeof(granny_uint32 *) + 
                                                    sizeof(granny_int32) + 
                                                    sizeof(granny_int32 *) + 
                                                    sizeof(granny_int32) + 
                                                    sizeof(granny_int32 *) + 
                                                    sizeof(granny_int32) + 
                                                    sizeof(granny_tri_annotation_set *)));

struct granny_defined_type
{
    granny_int32x UIID;
    char * Name;
    granny_data_type_definition * Definition;
};

struct granny_variant_builder;

struct granny_vertex_annotation_set
{
    char const * Name;
    granny_data_type_definition * VertexAnnotationType;
    granny_int32 VertexAnnotationCount;
    granny_uint8 * VertexAnnotations;
    granny_int32 IndicesMapFromVertexToAnnotation;
    granny_int32 VertexAnnotationIndexCount;
    granny_int32 * VertexAnnotationIndices;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_vertex_annotation_set) == (sizeof(char const *) + 
                                                             sizeof(granny_data_type_definition *) + 
                                                             sizeof(granny_int32) + 
                                                             sizeof(granny_uint8 *) + 
                                                             sizeof(granny_int32) + 
                                                             sizeof(granny_int32) + 
                                                             sizeof(granny_int32 *)));

struct granny_vertex_data
{
    granny_data_type_definition * VertexType;
    granny_int32 VertexCount;
    granny_uint8 * Vertices;
    granny_int32 VertexComponentNameCount;
    char const ** VertexComponentNames;
    granny_int32 VertexAnnotationSetCount;
    granny_vertex_annotation_set * VertexAnnotationSets;
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_vertex_data) == (sizeof(granny_data_type_definition *) + 
                                                   sizeof(granny_int32) + 
                                                   sizeof(granny_uint8 *) + 
                                                   sizeof(granny_int32) + 
                                                   sizeof(char const **) + 
                                                   sizeof(granny_int32) + 
                                                   sizeof(granny_vertex_annotation_set *)));

struct granny_p3_vertex
{
    granny_real32 Position[3];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_p3_vertex) == (sizeof(granny_real32) * 3));

struct granny_pt32_vertex
{
    granny_real32 Position[3];
    granny_real32 UV[2];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pt32_vertex) == (sizeof(granny_real32) * 3 + 
                                                   sizeof(granny_real32) * 2));

struct granny_pn33_vertex
{
    granny_real32 Position[3];
    granny_real32 Normal[3];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pn33_vertex) == (sizeof(granny_real32) * 3 + 
                                                   sizeof(granny_real32) * 3));

struct granny_png333_vertex
{
    granny_real32 Position[3];
    granny_real32 Normal[3];
    granny_real32 Tangent[3];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_png333_vertex) == (sizeof(granny_real32) * 3 + 
                                                     sizeof(granny_real32) * 3 + 
                                                     sizeof(granny_real32) * 3));

struct granny_pngt3332_vertex
{
    granny_real32 Position[3];
    granny_real32 Normal[3];
    granny_real32 Tangent[3];
    granny_real32 UV[2];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pngt3332_vertex) == (sizeof(granny_real32) * 3 + 
                                                       sizeof(granny_real32) * 3 + 
                                                       sizeof(granny_real32) * 3 + 
                                                       sizeof(granny_real32) * 2));

struct granny_pntg3323_vertex
{
    granny_real32 Position[3];
    granny_real32 Normal[3];
    granny_real32 UV[2];
    granny_real32 Tangent[3];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pntg3323_vertex) == (sizeof(granny_real32) * 3 + 
                                                       sizeof(granny_real32) * 3 + 
                                                       sizeof(granny_real32) * 2 + 
                                                       sizeof(granny_real32) * 3));

struct granny_pngb3333_vertex
{
    granny_real32 Position[3];
    granny_real32 Normal[3];
    granny_real32 Tangent[3];
    granny_real32 Binormal[3];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pngb3333_vertex) == (sizeof(granny_real32) * 3 + 
                                                       sizeof(granny_real32) * 3 + 
                                                       sizeof(granny_real32) * 3 + 
                                                       sizeof(granny_real32) * 3));

struct granny_pnt332_vertex
{
    granny_real32 Position[3];
    granny_real32 Normal[3];
    granny_real32 UV[2];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pnt332_vertex) == (sizeof(granny_real32) * 3 + 
                                                     sizeof(granny_real32) * 3 + 
                                                     sizeof(granny_real32) * 2));

struct granny_pngbt33332_vertex
{
    granny_real32 Position[3];
    granny_real32 Normal[3];
    granny_real32 Tangent[3];
    granny_real32 Binormal[3];
    granny_real32 UV[2];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pngbt33332_vertex) == (sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_real32) * 2));

struct granny_pnt333_vertex
{
    granny_real32 Position[3];
    granny_real32 Normal[3];
    granny_real32 UVW[3];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pnt333_vertex) == (sizeof(granny_real32) * 3 + 
                                                     sizeof(granny_real32) * 3 + 
                                                     sizeof(granny_real32) * 3));

struct granny_pngbt33333_vertex
{
    granny_real32 Position[3];
    granny_real32 Normal[3];
    granny_real32 Tangent[3];
    granny_real32 Binormal[3];
    granny_real32 UVW[3];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pngbt33333_vertex) == (sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_real32) * 3));

struct granny_pwn313_vertex
{
    granny_real32 Position[3];
    granny_uint32 BoneIndex;
    granny_real32 Normal[3];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pwn313_vertex) == (sizeof(granny_real32) * 3 + 
                                                     sizeof(granny_uint32) + 
                                                     sizeof(granny_real32) * 3));

struct granny_pwng3133_vertex
{
    granny_real32 Position[3];
    granny_uint32 BoneIndex;
    granny_real32 Normal[3];
    granny_real32 Tangent[3];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pwng3133_vertex) == (sizeof(granny_real32) * 3 + 
                                                       sizeof(granny_uint32) + 
                                                       sizeof(granny_real32) * 3 + 
                                                       sizeof(granny_real32) * 3));

struct granny_pwngt31332_vertex
{
    granny_real32 Position[3];
    granny_uint32 BoneIndex;
    granny_real32 Normal[3];
    granny_real32 Tangent[3];
    granny_real32 UV[2];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pwngt31332_vertex) == (sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_uint32) + 
                                                         sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_real32) * 2));

struct granny_pwngb31333_vertex
{
    granny_real32 Position[3];
    granny_uint32 BoneIndex;
    granny_real32 Normal[3];
    granny_real32 Tangent[3];
    granny_real32 Binormal[3];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pwngb31333_vertex) == (sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_uint32) + 
                                                         sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_real32) * 3));

struct granny_pwnt3132_vertex
{
    granny_real32 Position[3];
    granny_uint32 BoneIndex;
    granny_real32 Normal[3];
    granny_real32 UV[2];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pwnt3132_vertex) == (sizeof(granny_real32) * 3 + 
                                                       sizeof(granny_uint32) + 
                                                       sizeof(granny_real32) * 3 + 
                                                       sizeof(granny_real32) * 2));

struct granny_pwngbt313332_vertex
{
    granny_real32 Position[3];
    granny_uint32 BoneIndex;
    granny_real32 Normal[3];
    granny_real32 Tangent[3];
    granny_real32 Binormal[3];
    granny_real32 UV[2];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pwngbt313332_vertex) == (sizeof(granny_real32) * 3 + 
                                                           sizeof(granny_uint32) + 
                                                           sizeof(granny_real32) * 3 + 
                                                           sizeof(granny_real32) * 3 + 
                                                           sizeof(granny_real32) * 3 + 
                                                           sizeof(granny_real32) * 2));

struct granny_pwn323_vertex
{
    granny_real32 Position[3];
    granny_uint8 BoneWeights[2];
    granny_uint8 BoneIndices[2];
    granny_real32 Normal[3];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pwn323_vertex) == (sizeof(granny_real32) * 3 + 
                                                     sizeof(granny_uint8) * 2 + 
                                                     sizeof(granny_uint8) * 2 + 
                                                     sizeof(granny_real32) * 3));

struct granny_pwng3233_vertex
{
    granny_real32 Position[3];
    granny_uint8 BoneWeights[2];
    granny_uint8 BoneIndices[2];
    granny_real32 Normal[3];
    granny_real32 Tangent[3];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pwng3233_vertex) == (sizeof(granny_real32) * 3 + 
                                                       sizeof(granny_uint8) * 2 + 
                                                       sizeof(granny_uint8) * 2 + 
                                                       sizeof(granny_real32) * 3 + 
                                                       sizeof(granny_real32) * 3));

struct granny_pwngt32332_vertex
{
    granny_real32 Position[3];
    granny_uint8 BoneWeights[2];
    granny_uint8 BoneIndices[2];
    granny_real32 Normal[3];
    granny_real32 Tangent[3];
    granny_real32 UV[2];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pwngt32332_vertex) == (sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_uint8) * 2 + 
                                                         sizeof(granny_uint8) * 2 + 
                                                         sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_real32) * 2));

struct granny_pwngb32333_vertex
{
    granny_real32 Position[3];
    granny_uint8 BoneWeights[2];
    granny_uint8 BoneIndices[2];
    granny_real32 Normal[3];
    granny_real32 Tangent[3];
    granny_real32 Binormal[3];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pwngb32333_vertex) == (sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_uint8) * 2 + 
                                                         sizeof(granny_uint8) * 2 + 
                                                         sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_real32) * 3));

struct granny_pwnt3232_vertex
{
    granny_real32 Position[3];
    granny_uint8 BoneWeights[2];
    granny_uint8 BoneIndices[2];
    granny_real32 Normal[3];
    granny_real32 UV[2];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pwnt3232_vertex) == (sizeof(granny_real32) * 3 + 
                                                       sizeof(granny_uint8) * 2 + 
                                                       sizeof(granny_uint8) * 2 + 
                                                       sizeof(granny_real32) * 3 + 
                                                       sizeof(granny_real32) * 2));

struct granny_pwngbt323332_vertex
{
    granny_real32 Position[3];
    granny_uint8 BoneWeights[2];
    granny_uint8 BoneIndices[2];
    granny_real32 Normal[3];
    granny_real32 Tangent[3];
    granny_real32 Binormal[3];
    granny_real32 UV[2];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pwngbt323332_vertex) == (sizeof(granny_real32) * 3 + 
                                                           sizeof(granny_uint8) * 2 + 
                                                           sizeof(granny_uint8) * 2 + 
                                                           sizeof(granny_real32) * 3 + 
                                                           sizeof(granny_real32) * 3 + 
                                                           sizeof(granny_real32) * 3 + 
                                                           sizeof(granny_real32) * 2));

struct granny_pwn343_vertex
{
    granny_real32 Position[3];
    granny_uint8 BoneWeights[4];
    granny_uint8 BoneIndices[4];
    granny_real32 Normal[3];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pwn343_vertex) == (sizeof(granny_real32) * 3 + 
                                                     sizeof(granny_uint8) * 4 + 
                                                     sizeof(granny_uint8) * 4 + 
                                                     sizeof(granny_real32) * 3));

struct granny_pwng3433_vertex
{
    granny_real32 Position[3];
    granny_uint8 BoneWeights[4];
    granny_uint8 BoneIndices[4];
    granny_real32 Normal[3];
    granny_real32 Tangent[3];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pwng3433_vertex) == (sizeof(granny_real32) * 3 + 
                                                       sizeof(granny_uint8) * 4 + 
                                                       sizeof(granny_uint8) * 4 + 
                                                       sizeof(granny_real32) * 3 + 
                                                       sizeof(granny_real32) * 3));

struct granny_pwngt34332_vertex
{
    granny_real32 Position[3];
    granny_uint8 BoneWeights[4];
    granny_uint8 BoneIndices[4];
    granny_real32 Normal[3];
    granny_real32 Tangent[3];
    granny_real32 UV[2];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pwngt34332_vertex) == (sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_uint8) * 4 + 
                                                         sizeof(granny_uint8) * 4 + 
                                                         sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_real32) * 2));

struct granny_pwngb34333_vertex
{
    granny_real32 Position[3];
    granny_uint8 BoneWeights[4];
    granny_uint8 BoneIndices[4];
    granny_real32 Normal[3];
    granny_real32 Tangent[3];
    granny_real32 Binormal[3];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pwngb34333_vertex) == (sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_uint8) * 4 + 
                                                         sizeof(granny_uint8) * 4 + 
                                                         sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_real32) * 3 + 
                                                         sizeof(granny_real32) * 3));

struct granny_pwnt3432_vertex
{
    granny_real32 Position[3];
    granny_uint8 BoneWeights[4];
    granny_uint8 BoneIndices[4];
    granny_real32 Normal[3];
    granny_real32 UV[2];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pwnt3432_vertex) == (sizeof(granny_real32) * 3 + 
                                                       sizeof(granny_uint8) * 4 + 
                                                       sizeof(granny_uint8) * 4 + 
                                                       sizeof(granny_real32) * 3 + 
                                                       sizeof(granny_real32) * 2));

struct granny_pwngbt343332_vertex
{
    granny_real32 Position[3];
    granny_uint8 BoneWeights[4];
    granny_uint8 BoneIndices[4];
    granny_real32 Normal[3];
    granny_real32 Tangent[3];
    granny_real32 Binormal[3];
    granny_real32 UV[2];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_pwngbt343332_vertex) == (sizeof(granny_real32) * 3 + 
                                                           sizeof(granny_uint8) * 4 + 
                                                           sizeof(granny_uint8) * 4 + 
                                                           sizeof(granny_real32) * 3 + 
                                                           sizeof(granny_real32) * 3 + 
                                                           sizeof(granny_real32) * 3 + 
                                                           sizeof(granny_real32) * 2));

struct granny_vertex_weight_arrays
{
    granny_real32 BoneWeights[GrannyMaximumWeightCount];
    granny_uint32 BoneIndices[GrannyMaximumWeightCount];
} GrannyAlignAttribute;
GrannyTypeSizeCheck(sizeof(granny_vertex_weight_arrays) == (sizeof(granny_real32) * GrannyMaximumWeightCount + 
                                                            sizeof(granny_uint32) * GrannyMaximumWeightCount));

struct granny_world_pose;

/* granny_transform.h bindings: */
GRANNY_DYNLINK(void) GrannySetTransform(granny_transform * Result,
                                        granny_real32 const * Position3,
                                        granny_real32 const * Orientation4,
                                        granny_real32 const * ScaleShear3x3);
GRANNY_DYNLINK(void) GrannySetTransformWithIdentityCheck(granny_transform * Result,
                                                         granny_real32 const * Position3,
                                                         granny_real32 const * Orientation4,
                                                         granny_real32 const * ScaleShear3x3);
GRANNY_DYNLINK(void) GrannyMakeIdentity(granny_transform * Result);
GRANNY_DYNLINK(void) GrannyZeroTransform(granny_transform * Result);
GRANNY_DYNLINK(granny_real32) GrannyGetTransformDeterminant(granny_transform const * Transform);
GRANNY_DYNLINK(void) GrannyTransformVectorInPlace(granny_real32 * Result,
                                                  granny_transform const * Transform);
GRANNY_DYNLINK(void) GrannyTransformVectorInPlaceTransposed(granny_real32 * Result,
                                                            granny_transform const * Transform);
GRANNY_DYNLINK(void) GrannyTransformVector(granny_real32 * Dest,
                                           granny_transform const * Transform,
                                           granny_real32 const * Source);
GRANNY_DYNLINK(void) GrannyTransformPointInPlace(granny_real32 * Result,
                                                 granny_transform const * Transform);
GRANNY_DYNLINK(void) GrannyTransformPoint(granny_real32 * Dest,
                                          granny_transform const * Transform,
                                          granny_real32 const * Source);
GRANNY_DYNLINK(void) GrannyPreMultiplyBy(granny_transform * Transform,
                                         granny_transform const * PreMult);
GRANNY_DYNLINK(void) GrannyPostMultiplyBy(granny_transform * Transform,
                                          granny_transform const * PostMult);
GRANNY_DYNLINK(void) GrannyMultiply(granny_transform * Result,
                                    granny_transform const * A,
                                    granny_transform const * B);
GRANNY_DYNLINK(void) GrannyLinearBlendTransform(granny_transform * Result,
                                                granny_transform const * A,
                                                granny_real32 t,
                                                granny_transform const * B);
GRANNY_DYNLINK(void) GrannyBuildInverse(granny_transform * Result,
                                        granny_transform const * Source);
GRANNY_DYNLINK(void) GrannySimilarityTransform(granny_transform * Result,
                                               granny_real32 const * Affine3,
                                               granny_real32 const * Linear3x3,
                                               granny_real32 const * InverseLinear3x3);
GRANNY_DYNLINK(void) GrannyBuildCompositeTransform(granny_transform const * Transform,
                                                   granny_int32 Stride,
                                                   granny_real32 * Composite3x3);
GRANNY_DYNLINK(void) GrannyBuildCompositeTransform4x4(granny_transform const * Transform,
                                                      granny_real32 * Composite4x4);
GRANNY_DYNLINK(void) GrannyBuildCompositeTransform4x3(granny_transform const * Transform,
                                                      granny_real32 * Composite4x3);

/* granny_model.h bindings: */
GRANNY_DYNLINK(void) GrannyGetModelInitialPlacement4x4(granny_model const * Model,
                                                       granny_real32 * Placement4x4);
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyModelMeshBindingType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyModelType;

/* granny_model_control_binding.h bindings: */
GRANNY_DYNLINK(granny_model_control_binding *) GrannyModelControlsBegin(granny_model_instance * Model);
GRANNY_DYNLINK(granny_model_control_binding *) GrannyModelControlsNext(granny_model_control_binding * Binding);
GRANNY_DYNLINK(granny_model_control_binding *) GrannyModelControlsEnd(granny_model_instance * Model);
GRANNY_DYNLINK(granny_model_control_binding *) GrannyControlModelsBegin(granny_control * Control);
GRANNY_DYNLINK(granny_model_control_binding *) GrannyControlModelsNext(granny_model_control_binding * Binding);
GRANNY_DYNLINK(granny_model_control_binding *) GrannyControlModelsEnd(granny_control * Control);
GRANNY_DYNLINK(granny_model_instance *) GrannyGetModelInstanceFromBinding(granny_model_control_binding * Binding);
GRANNY_DYNLINK(granny_control *) GrannyGetControlFromBinding(granny_model_control_binding * Binding);

/* granny_model_instance.h bindings: */
GRANNY_DYNLINK(granny_model_instance *) GrannyInstantiateModel(granny_model const * Model);
GRANNY_DYNLINK(void) GrannyFreeModelInstance(granny_model_instance * ModelInstance);
GRANNY_DYNLINK(granny_model *) GrannyGetSourceModel(granny_model_instance const * Model);
GRANNY_DYNLINK(granny_skeleton *) GrannyGetSourceSkeleton(granny_model_instance const * Model);
GRANNY_DYNLINK(void) GrannySetModelClock(granny_model_instance const * ModelInstance,
                                         granny_real32 NewClock);
GRANNY_DYNLINK(void) GrannyFreeCompletedModelControls(granny_model_instance const * ModelInstance);
GRANNY_DYNLINK(void) GrannyAccumulateModelAnimationsLODSparse(granny_model_instance const * ModelInstance,
                                                              granny_int32x FirstBone,
                                                              granny_int32x BoneCount,
                                                              granny_local_pose * Result,
                                                              granny_real32 AllowedError,
                                                              granny_int32x const * SparseBoneArray);
GRANNY_DYNLINK(void) GrannySampleModelAnimationsLODSparse(granny_model_instance const * ModelInstance,
                                                          granny_int32x FirstBone,
                                                          granny_int32x BoneCount,
                                                          granny_local_pose * Result,
                                                          granny_real32 AllowedError,
                                                          granny_int32x const * SparseBoneArray);
GRANNY_DYNLINK(bool) GrannySampleSingleModelAnimationLODSparse(granny_model_instance const * ModelInstance,
                                                               granny_control const * Control,
                                                               granny_int32x FirstBone,
                                                               granny_int32x BoneCount,
                                                               granny_local_pose * Result,
                                                               granny_real32 AllowedError,
                                                               granny_int32x const * SparseBoneArray);
GRANNY_DYNLINK(void) GrannySampleModelAnimationsAcceleratedLOD(granny_model_instance const * ModelInstance,
                                                               granny_int32x BoneCount,
                                                               granny_real32 const * Offset4x4,
                                                               granny_local_pose * Scratch,
                                                               granny_world_pose * Result,
                                                               granny_real32 AllowedError);
GRANNY_DYNLINK(void) GrannyAccumulateModelAnimations(granny_model_instance const * ModelInstance,
                                                     granny_int32x FirstBone,
                                                     granny_int32x BoneCount,
                                                     granny_local_pose * Result);
GRANNY_DYNLINK(void) GrannySampleModelAnimations(granny_model_instance const * ModelInstance,
                                                 granny_int32x FirstBone,
                                                 granny_int32x BoneCount,
                                                 granny_local_pose * Result);
GRANNY_DYNLINK(bool) GrannySampleSingleModelAnimation(granny_model_instance const * ModelInstance,
                                                      granny_control const * Control,
                                                      granny_int32x FirstBone,
                                                      granny_int32x BoneCount,
                                                      granny_local_pose * Result);
GRANNY_DYNLINK(void) GrannySampleModelAnimationsAccelerated(granny_model_instance const * ModelInstance,
                                                            granny_int32x BoneCount,
                                                            granny_real32 const * Offset4x4,
                                                            granny_local_pose * Scratch,
                                                            granny_world_pose * Result);
GRANNY_DYNLINK(void) GrannyAccumulateModelAnimationsLOD(granny_model_instance const * ModelInstance,
                                                        granny_int32x FirstBone,
                                                        granny_int32x BoneCount,
                                                        granny_local_pose * Result,
                                                        granny_real32 AllowedError);
GRANNY_DYNLINK(void) GrannySampleModelAnimationsLOD(granny_model_instance const * ModelInstance,
                                                    granny_int32x FirstBone,
                                                    granny_int32x BoneCount,
                                                    granny_local_pose * Result,
                                                    granny_real32 AllowedError);
GRANNY_DYNLINK(bool) GrannySampleSingleModelAnimationLOD(granny_model_instance const * ModelInstance,
                                                         granny_control const * Control,
                                                         granny_int32x FirstBone,
                                                         granny_int32x BoneCount,
                                                         granny_local_pose * Result,
                                                         granny_real32 AllowedError);
GRANNY_DYNLINK(void) GrannyGetRootMotionVectors(granny_model_instance const * ModelInstance,
                                                granny_real32 SecondsElapsed,
                                                granny_real32 * ResultTranslation3,
                                                granny_real32 * ResultRotation3,
                                                bool Inverse);
GRANNY_DYNLINK(void) GrannyClipRootMotionVectors(granny_real32 const * Translation3,
                                                 granny_real32 const * Rotation3,
                                                 granny_uint32 AllowableDOFs,
                                                 granny_real32 * AllowedTranslation3,
                                                 granny_real32 * AllowedRotation3,
                                                 granny_real32 * DisallowedTranslation3,
                                                 granny_real32 * DisallowedRotation3);
GRANNY_DYNLINK(void) GrannyApplyRootMotionVectorsToMatrix(granny_real32 const * ModelMatrix4x4,
                                                          granny_real32 const * Translation3,
                                                          granny_real32 const * Rotation3,
                                                          granny_real32 * DestMatrix4x4);
GRANNY_DYNLINK(void) GrannyApplyRootMotionVectorsToLocalPose(granny_local_pose * Pose,
                                                             granny_real32 const * Translation3,
                                                             granny_real32 const * Rotation3);
GRANNY_DYNLINK(void) GrannyUpdateModelMatrix(granny_model_instance const * ModelInstance,
                                             granny_real32 SecondsElapsed,
                                             granny_real32 const * ModelMatrix4x4,
                                             granny_real32 * DestMatrix4x4,
                                             bool Inverse);
GRANNY_DYNLINK(void **) GrannyGetModelUserDataArray(granny_model_instance const * ModelInstance);
GRANNY_DYNLINK(granny_model_instance *) GrannyGetGlobalModelInstancesBegin(void);
GRANNY_DYNLINK(granny_model_instance *) GrannyGetGlobalModelInstancesEnd(void);
GRANNY_DYNLINK(granny_model_instance *) GrannyGetGlobalNextModelInstance(granny_model_instance * Instance);

/* granny_data_type_definition.h bindings: */
GRANNY_DYNLINK(granny_int32x) GrannyGetMemberUnitSize(granny_data_type_definition const * MemberType);
GRANNY_DYNLINK(granny_int32x) GrannyGetMemberTypeSize(granny_data_type_definition const * MemberType);
GRANNY_DYNLINK(granny_int32x) GrannyGetTotalObjectSize(granny_data_type_definition const * TypeDefinition);
GRANNY_DYNLINK(granny_int32x) GrannyGetTotalTypeSize(granny_data_type_definition const * TypeDefinition);
GRANNY_DYNLINK(char const *) GrannyGetMemberTypeName(granny_member_type Type);
GRANNY_DYNLINK(char const *) GrannyGetMemberCTypeName(granny_member_type Type);
GRANNY_DYNLINK(bool) GrannyMemberHasPointers(granny_data_type_definition const * MemberType);
GRANNY_DYNLINK(bool) GrannyTypeHasPointers(granny_data_type_definition const * TypeDefinition);
GRANNY_DYNLINK(granny_uint32) GrannyGetMemberMarshalling(granny_data_type_definition const * MemberType);
GRANNY_DYNLINK(granny_uint32) GrannyGetObjectMarshalling(granny_data_type_definition const * TypeDefinition);
GRANNY_DYNLINK(bool) GrannyIsMixedMarshalling(granny_uint32x Marshalling);
GRANNY_DYNLINK(granny_intaddrx) GrannyMakeEmptyDataTypeMember(granny_data_type_definition const * MemberType,
                                                              void * Member);
GRANNY_DYNLINK(granny_intaddrx) GrannyMakeEmptyDataTypeObject(granny_data_type_definition const * TypeDefinition,
                                                              void * Object);
GRANNY_DYNLINK(granny_int32x) GrannyGetMemberArrayWidth(granny_data_type_definition const * MemberType);
GRANNY_DYNLINK(granny_int32x) GrannyGetTypeTableCount(granny_data_type_definition const ** TypeTable);
GRANNY_DYNLINK(bool) GrannyDataTypesAreEqual(granny_data_type_definition const * A,
                                             granny_data_type_definition const * B);
GRANNY_DYNLINK(bool) GrannyDataTypesAreEqualWithNames(granny_data_type_definition const * A,
                                                      granny_data_type_definition const * B);
GRANNY_DYNLINK(bool) GrannyDataTypesAreEqualWithNameCallback(granny_data_type_definition const * A,
                                                             granny_data_type_definition const * B,
                                                             granny_string_comparison_callback * Callback);
GRANNY_DYNLINK(granny_data_type_definition *) GrannyDataTypeBeginsWith(granny_data_type_definition const * Type,
                                                                       granny_data_type_definition const * Prefix);
GRANNY_DYNLINK(void) GrannyReverseTypeArray(granny_data_type_definition const * Type,
                                            granny_int32x Count,
                                            void * TypeArray);
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyEmptyType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyStringType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyInt16Type;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyInt32Type;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyUInt8Type;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyUInt16Type;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyUInt32Type;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyReal32Type;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyTripleType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyQuadType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyTransformType;

/* granny_curve.h bindings: */
GRANNY_DYNLINK(void) GrannyCurveInitializeFormat(granny_curve2 * Curve);
GRANNY_DYNLINK(bool) GrannyCurveFormatIsInitializedCorrectly(granny_curve2 const * Curve,
                                                             bool CheckTypes);
GRANNY_DYNLINK(bool) GrannyCurveIsKeyframed(granny_curve2 const * Curve);
GRANNY_DYNLINK(bool) GrannyCurveIsIdentity(granny_curve2 const * Curve);
GRANNY_DYNLINK(bool) GrannyCurveIsConstantOrIdentity(granny_curve2 const * Curve);
GRANNY_DYNLINK(bool) GrannyCurveIsConstantNotIdentity(granny_curve2 const * Curve);
GRANNY_DYNLINK(granny_int32x) GrannyCurveGetKnotCount(granny_curve2 const * Curve);
GRANNY_DYNLINK(granny_int32x) GrannyCurveGetDimension(granny_curve2 const * Curve);
GRANNY_DYNLINK(granny_int32x) GrannyCurveGetDegree(granny_curve2 const * Curve);
GRANNY_DYNLINK(granny_data_type_definition const *) GrannyCurveGetDataTypeDefinition(granny_curve2 const * Curve);
GRANNY_DYNLINK(granny_curve_builder *) GrannyBeginCurve(granny_data_type_definition const * TypeDefinition,
                                                        granny_int32x Degree,
                                                        granny_int32x Dimension,
                                                        granny_int32x KnotCount);
GRANNY_DYNLINK(granny_curve_builder *) GrannyBeginCurveCopy(granny_curve2 const * SourceCurve);
GRANNY_DYNLINK(void) GrannyPushCurveKnotArray(granny_curve_builder * Builder,
                                              granny_real32 const * KnotArray);
GRANNY_DYNLINK(void) GrannyPushCurveControlArray(granny_curve_builder * Builder,
                                                 granny_real32 const * ControlArray);
GRANNY_DYNLINK(void) GrannyPushCurveSampleArrays(granny_curve_builder * Builder,
                                                 granny_int32x SampleCount,
                                                 granny_int32x Dimension,
                                                 granny_real32 const * TransformedSamples,
                                                 granny_real32 const * OriginalSamples);
GRANNY_DYNLINK(granny_int32x) GrannyGetResultingCurveSize(granny_curve_builder const * Builder);
GRANNY_DYNLINK(granny_curve2 *) GrannyEndCurveInPlace(granny_curve_builder * Builder,
                                                      void * Memory);
GRANNY_DYNLINK(granny_curve2 *) GrannyEndCurve(granny_curve_builder * Builder);
GRANNY_DYNLINK(void) GrannyAbortCurveBuilder(granny_curve_builder * Builder);
GRANNY_DYNLINK(void) GrannyFreeCurve(granny_curve2 * Curve);
GRANNY_DYNLINK(granny_int32x) GrannyGetResultingCurveDataSize(granny_curve_builder const * Builder);
GRANNY_DYNLINK(granny_curve2 *) GrannyEndCurveDataInPlace(granny_curve_builder * Builder,
                                                          granny_curve2 * Curve,
                                                          void * CurveDataMemory);
GRANNY_DYNLINK(bool) GrannyCurveIsTypeDaK32fC32f(granny_curve2 const * SrcCurve);
GRANNY_DYNLINK(granny_curve_data_da_k32f_c32f *) GrannyCurveGetContentsOfDaK32fC32f(granny_curve2 const * SrcCurve);
GRANNY_DYNLINK(granny_curve2 *) GrannyCurveConvertToDaK32fC32f(granny_curve2 const * SrcCurve,
                                                               granny_real32 const * IdentityVector);
GRANNY_DYNLINK(granny_int32x) GrannyGetResultingDaK32fC32fCurveSize(granny_curve2 const * SrcCurve);
GRANNY_DYNLINK(granny_curve2 *) GrannyCurveConvertToDaK32fC32fInPlace(granny_curve2 const * SrcCurve,
                                                                      void * Memory,
                                                                      granny_real32 const * IdentityVector);
GRANNY_DYNLINK(void) GrannyCurveMakeStaticDaK32fC32f(granny_curve2 * Curve,
                                                     granny_curve_data_da_k32f_c32f * CurveData,
                                                     granny_int32x KnotCount,
                                                     granny_int32x Degree,
                                                     granny_int32x Dimension,
                                                     granny_real32 const * Knots,
                                                     granny_real32 const * Controls);
GRANNY_DYNLINK(granny_real32) GrannyCurveExtractKnotValue(granny_curve2 const * Curve,
                                                          granny_int32x KnotIndex,
                                                          granny_real32 * ControlResult,
                                                          granny_real32 const * IdentityVector);
GRANNY_DYNLINK(void) GrannyCurveExtractKnotValues(granny_curve2 const * Curve,
                                                  granny_int32x KnotIndexStart,
                                                  granny_int32x KnotCount,
                                                  granny_real32 * KnotResults,
                                                  granny_real32 * ControlResults,
                                                  granny_real32 const * IdentityVector);
GRANNY_DYNLINK(void) GrannyCurveSetAllKnotValues(granny_curve2 * Curve,
                                                 granny_int32x KnotCount,
                                                 granny_int32x Dimension,
                                                 granny_real32 const * KnotSrc,
                                                 granny_real32 const * ControlSrc);
GRANNY_DYNLINK(void) GrannyCurveScaleOffsetSwizzle(granny_curve2 * Curve,
                                                   granny_int32x Dimension,
                                                   granny_real32 const * Scales,
                                                   granny_real32 const * Offsets,
                                                   granny_int32x const * Swizzles);
GRANNY_DYNLINK(granny_int32x) GrannyCurveFindKnot(granny_curve2 const * Curve,
                                                  granny_real32 t);
GRANNY_DYNLINK(granny_int32x) GrannyCurveFindCloseKnot(granny_curve2 const * Curve,
                                                       granny_real32 t,
                                                       granny_int32x StartingKnotIndex);
GRANNY_DYNLINK(granny_int32) GrannyCurveGetSize(granny_curve2 const * Curve);
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyOldCurveType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyCurve2Type;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyCurveDataHeaderType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyCurveDataDaKeyframes32fType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyCurveDataDaK32fC32fType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyCurveDataDaK16uC16uType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyCurveDataDaK8uC8uType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyCurveDataD3K16uC16uType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyCurveDataD3K8uC8uType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyCurveDataD4nK16uC15uType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyCurveDataD4nK8uC7uType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyCurveDataDaIdentityType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyCurveDataDaConstant32fType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyCurveDataD3Constant32fType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyCurveDataD4Constant32fType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyCurveDataD9I1K16uC16uType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyCurveDataD9I3K16uC16uType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyCurveDataD9I1K8uC8uType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyCurveDataD9I3K8uC8uType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyCurveDataD3I1K32fC32fType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyCurveDataD3I1K16uC16uType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyCurveDataD3I1K8uC8uType;
extern GRANNY_DYNLINKDATA(granny_real32 *) GrannyCurveIdentityPosition;
extern GRANNY_DYNLINKDATA(granny_real32 *) GrannyCurveIdentityNormal;
extern GRANNY_DYNLINKDATA(granny_real32 *) GrannyCurveIdentityOrientation;
extern GRANNY_DYNLINKDATA(granny_real32 *) GrannyCurveIdentityScaleShear;
extern GRANNY_DYNLINKDATA(granny_real32 *) GrannyCurveIdentityScale;
extern GRANNY_DYNLINKDATA(granny_real32 *) GrannyCurveIdentityShear;

/* granny_material.h bindings: */
GRANNY_DYNLINK(granny_texture *) GrannyGetMaterialTextureByType(granny_material const * Material,
                                                                granny_material_texture_type Type);
GRANNY_DYNLINK(granny_material *) GrannyGetTexturedMaterialByChannelName(granny_material const * Material,
                                                                         char const * ChannelName);
GRANNY_DYNLINK(granny_texture *) GrannyGetMaterialTextureByChannelName(granny_material const * Material,
                                                                       char const * ChannelName);
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyMaterialMapType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyMaterialType;

/* granny_pixel_layout.h bindings: */
GRANNY_DYNLINK(bool) GrannyPixelLayoutsAreEqual(granny_pixel_layout const * Operand0,
                                                granny_pixel_layout const * Operand1);
GRANNY_DYNLINK(bool) GrannyPixelLayoutHasAlpha(granny_pixel_layout const * Layout);
GRANNY_DYNLINK(void) GrannySetStockSpecification(granny_pixel_layout * Layout,
                                                 granny_int32 const * BitsForComponent,
                                                 granny_int32 const * ComponentPlacement);
GRANNY_DYNLINK(void) GrannySetStockRGBASpecification(granny_pixel_layout * Layout,
                                                     granny_int32x RedBits,
                                                     granny_int32x GreenBits,
                                                     granny_int32x BlueBits,
                                                     granny_int32x AlphaBits);
GRANNY_DYNLINK(void) GrannySetStockBGRASpecification(granny_pixel_layout * Layout,
                                                     granny_int32x RedBits,
                                                     granny_int32x GreenBits,
                                                     granny_int32x BlueBits,
                                                     granny_int32x AlphaBits);
GRANNY_DYNLINK(void) GrannySwapRGBAToBGRA(granny_pixel_layout * Layout);
GRANNY_DYNLINK(void) GrannyConvertPixelFormat(granny_int32x Width,
                                              granny_int32x Height,
                                              granny_pixel_layout const * SourceLayout,
                                              granny_int32x SourceStride,
                                              void const * SourceBits,
                                              granny_pixel_layout const * DestLayout,
                                              granny_int32x DestStride,
                                              void * DestBits);
GRANNY_DYNLINK(void) GrannyARGB8888SwizzleNGC(granny_uint32 Width,
                                              granny_uint32 Height,
                                              granny_uint32 SourceStride,
                                              void * SourceBits,
                                              void * DestBits);
GRANNY_DYNLINK(void) GrannyAll16SwizzleNGC(granny_uint32 Width,
                                           granny_uint32 Height,
                                           granny_uint32 SourceStride,
                                           void * SourceBits,
                                           void * DestBits);
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPixelLayoutType;
extern GRANNY_DYNLINKDATA(granny_pixel_layout const  *) GrannyRGB555PixelFormat;
extern GRANNY_DYNLINKDATA(granny_pixel_layout const  *) GrannyRGB565PixelFormat;
extern GRANNY_DYNLINKDATA(granny_pixel_layout const  *) GrannyRGBA5551PixelFormat;
extern GRANNY_DYNLINKDATA(granny_pixel_layout const  *) GrannyRGBA4444PixelFormat;
extern GRANNY_DYNLINKDATA(granny_pixel_layout const  *) GrannyRGB888PixelFormat;
extern GRANNY_DYNLINKDATA(granny_pixel_layout const  *) GrannyRGBA8888PixelFormat;
extern GRANNY_DYNLINKDATA(granny_pixel_layout const  *) GrannyARGB8888PixelFormat;
extern GRANNY_DYNLINKDATA(granny_pixel_layout const  *) GrannyBGR555PixelFormat;
extern GRANNY_DYNLINKDATA(granny_pixel_layout const  *) GrannyBGR565PixelFormat;
extern GRANNY_DYNLINKDATA(granny_pixel_layout const  *) GrannyBGRA5551PixelFormat;
extern GRANNY_DYNLINKDATA(granny_pixel_layout const  *) GrannyBGRA4444PixelFormat;
extern GRANNY_DYNLINKDATA(granny_pixel_layout const  *) GrannyBGR888PixelFormat;
extern GRANNY_DYNLINKDATA(granny_pixel_layout const  *) GrannyBGRA8888PixelFormat;

/* granny_animation.h bindings: */
GRANNY_DYNLINK(bool) GrannyFindTrackGroupForModel(granny_animation const * Animation,
                                                  char const * ModelName,
                                                  granny_int32x * TrackGroupIndex);
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyAnimationType;

/* granny_animation_binding.h bindings: */
GRANNY_DYNLINK(void) GrannyMakeDefaultAnimationBindingID(granny_animation_binding_identifier * ID,
                                                         granny_animation const * Animation,
                                                         granny_int32x TrackGroupIndex);
GRANNY_DYNLINK(granny_animation_binding *) GrannyAcquireAnimationBindingFromID(granny_animation_binding_identifier * ID);
GRANNY_DYNLINK(granny_animation_binding *) GrannyAcquireAnimationBinding(granny_animation_binding * Binding);
GRANNY_DYNLINK(void) GrannyReleaseAnimationBinding(granny_animation_binding * Binding);
GRANNY_DYNLINK(granny_int32x) GrannyGetMaximumAnimationBindingCount(void);
GRANNY_DYNLINK(void) GrannySetMaximumAnimationBindingCount(granny_int32x BindingCountMax);
GRANNY_DYNLINK(void) GrannyFlushAllUnusedAnimationBindings(void);
GRANNY_DYNLINK(void) GrannyFlushAllBindingsForAnimation(granny_animation const * Animation);
GRANNY_DYNLINK(void) GrannyFlushAnimationBinding(granny_animation_binding * Binding);
GRANNY_DYNLINK(granny_animation_binding *) GrannyGetFirstBindingForAnimation(granny_animation const * Animation);
GRANNY_DYNLINK(granny_animation_binding *) GrannyGetNextBindingForAnimation(granny_animation const * Animation,
                                                                            granny_animation_binding * Binding);
GRANNY_DYNLINK(granny_animation_binding *) GrannyGetFirstUnusedAnimationBinding(void);
GRANNY_DYNLINK(granny_animation_binding *) GrannyGetNextUnusedAnimationBinding(granny_animation_binding * Binding);
GRANNY_DYNLINK(bool) GrannyIsAnimationUsed(granny_animation const * Animation);
GRANNY_DYNLINK(void) GrannyRemapAnimationBindingPointers(granny_animation_binding * Binding,
                                                         granny_animation const * NewAnimationPointer);
GRANNY_DYNLINK(void) GrannyRemapAllAnimationBindingPointers(granny_animation const * OldAnimationPointer,
                                                            granny_animation const * NewAnimationPointer);
GRANNY_DYNLINK(void) GrannyGetAnimationBindingCacheStatus(granny_animation_binding_cache_status * Result);
GRANNY_DYNLINK(void) GrannyCalculateLODErrorValues(granny_animation_binding * Binding,
                                                   granny_mesh_binding const * MeshBinding,
                                                   bool AnimationHasScales,
                                                   granny_real32 ManualScaler);
GRANNY_DYNLINK(void) GrannyCalculateLODErrorValuesAllBindings(granny_model_instance * ModelInstance,
                                                              granny_mesh_binding const * MeshBinding,
                                                              bool AnimationHasScales,
                                                              granny_real32 ManualScaler);
GRANNY_DYNLINK(granny_real32) GrannyGetLODErrorValue(granny_animation_binding const * AnimationBinding,
                                                     granny_int32x SkeletonBoneIndex);
GRANNY_DYNLINK(void) GrannySetLODErrorValue(granny_animation_binding * AnimationBinding,
                                            granny_int32x SkeletonBoneIndex,
                                            granny_real32 NewError);
GRANNY_DYNLINK(void) GrannyCopyLODErrorValuesToAnimation(granny_animation_binding * Binding,
                                                         granny_real32 ManualScaler,
                                                         bool OnlyCopyIfGreater);
GRANNY_DYNLINK(void) GrannyCopyLODErrorValuesToAllAnimations(granny_control * Control,
                                                             granny_real32 ManualScaler,
                                                             bool OnlyCopyIfGreater);
GRANNY_DYNLINK(void) GrannyCopyLODErrorValuesFromAnimation(granny_animation_binding * Binding,
                                                           granny_real32 ManualScaler);
GRANNY_DYNLINK(void) GrannyCopyLODErrorValuesFromAllAnimations(granny_control * Control,
                                                               granny_real32 ManualScaler);
GRANNY_DYNLINK(void) GrannySetAllLODErrorValues(granny_animation_binding * AnimationBinding,
                                                granny_real32 NewValue);
GRANNY_DYNLINK(void) GrannyResetLODErrorValues(granny_animation_binding * AnimationBinding);
GRANNY_DYNLINK(granny_animation_lod_builder *) GrannyCalculateAnimationLODBegin(granny_animation const * Animation,
                                                                                granny_real32 ManualScaler);
GRANNY_DYNLINK(void) GrannyCalculateAnimationLODAddMeshBinding(granny_animation_lod_builder * Builder,
                                                               granny_model const * Model,
                                                               granny_mesh_binding const * MeshBinding,
                                                               granny_real32 ManualScaler);
GRANNY_DYNLINK(void) GrannyCalculateAnimationLODEnd(granny_animation_lod_builder * Builder);
GRANNY_DYNLINK(void) GrannyCalculateAnimationLODCleanup(granny_animation_lod_builder * Builder);

/* granny_art_tool_info.h bindings: */
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyArtToolInfoType;

/* granny_basis_conversion.h bindings: */
GRANNY_DYNLINK(bool) GrannyComputeBasisConversion(granny_file_info const * FileInfo,
                                                  granny_real32 DesiredUnitsPerMeter,
                                                  granny_real32 const * DesiredOrigin3,
                                                  granny_real32 const * DesiredRight3,
                                                  granny_real32 const * DesiredUp3,
                                                  granny_real32 const * DesiredBack3,
                                                  granny_real32 * ResultAffine3,
                                                  granny_real32 * ResultLinear3x3,
                                                  granny_real32 * ResultInverseLinear3x3);
GRANNY_DYNLINK(void) GrannyTransformMesh(granny_mesh * Mesh,
                                         granny_real32 const * Affine3,
                                         granny_real32 const * Linear3x3,
                                         granny_real32 const * InverseLinear3x3,
                                         granny_real32 AffineTolerance,
                                         granny_real32 LinearTolerance,
                                         granny_uint32x Flags);
GRANNY_DYNLINK(void) GrannyTransformSkeleton(granny_skeleton * Skeleton,
                                             granny_real32 const * Affine3,
                                             granny_real32 const * Linear3x3,
                                             granny_real32 const * InverseLinear3x3,
                                             granny_real32 AffineTolerance,
                                             granny_real32 LinearTolerance,
                                             granny_uint32x Flags);
GRANNY_DYNLINK(void) GrannyTransformModel(granny_model * Model,
                                          granny_real32 const * Affine3,
                                          granny_real32 const * Linear3x3,
                                          granny_real32 const * InverseLinear3x3,
                                          granny_real32 AffineTolerance,
                                          granny_real32 LinearTolerance,
                                          granny_uint32x Flags);
GRANNY_DYNLINK(void) GrannyTransformAnimation(granny_animation * Animation,
                                              granny_real32 const * Affine3,
                                              granny_real32 const * Linear3x3,
                                              granny_real32 const * InverseLinear3x3,
                                              granny_real32 AffineTolerance,
                                              granny_real32 LinearTolerance,
                                              granny_uint32x Flags);
GRANNY_DYNLINK(void) GrannyTransformFile(granny_file_info * FileInfo,
                                         granny_real32 const * Affine3,
                                         granny_real32 const * Linear3x3,
                                         granny_real32 const * InverseLinear3x3,
                                         granny_real32 AffineTolerance,
                                         granny_real32 LinearTolerance,
                                         granny_uint32x Flags);
GRANNY_DYNLINK(void) GrannyResortAllAnimationTrackGroups(granny_animation * Animation);
GRANNY_DYNLINK(void) GrannyResortAllFileTrackGroups(granny_file_info * FileInfo);

/* granny_bink.h bindings: */
GRANNY_DYNLINK(granny_pixel_layout const *) GrannyGetBinkPixelLayout(bool Alpha);
GRANNY_DYNLINK(granny_int32x) GrannyGetMaximumBinkImageSize(granny_int32x Width,
                                                            granny_int32x Height,
                                                            granny_uint32x Flags,
                                                            granny_int32x Compression);
GRANNY_DYNLINK(granny_int32x) GrannyBinkCompressTexture(granny_int32x Width,
                                                        granny_int32x Height,
                                                        granny_int32x SourceStride,
                                                        void const * Source,
                                                        granny_uint32x Flags,
                                                        granny_int32x Compression,
                                                        void * Dest);
GRANNY_DYNLINK(void) GrannyBinkDecompressTexture(granny_int32x Width,
                                                 granny_int32x Height,
                                                 granny_uint32x Flags,
                                                 granny_int32x SourceSize,
                                                 void const * Source,
                                                 granny_pixel_layout const * DestLayout,
                                                 granny_int32x DestStride,
                                                 void * Dest);

/* granny_bspline.h bindings: */
GRANNY_DYNLINK(void) GrannySampleBSpline0x1(granny_real32 const * ti,
                                            granny_real32 const * pi,
                                            granny_real32 t,
                                            granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline0x2(granny_real32 const * ti,
                                            granny_real32 const * pi,
                                            granny_real32 t,
                                            granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline0x3(granny_real32 const * ti,
                                            granny_real32 const * pi,
                                            granny_real32 t,
                                            granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline0x4(granny_real32 const * ti,
                                            granny_real32 const * pi,
                                            granny_real32 t,
                                            granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline0x9(granny_real32 const * ti,
                                            granny_real32 const * pi,
                                            granny_real32 t,
                                            granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline1x1(granny_real32 const * ti,
                                            granny_real32 const * pi,
                                            granny_real32 t,
                                            granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline1x2(granny_real32 const * ti,
                                            granny_real32 const * pi,
                                            granny_real32 t,
                                            granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline1x3(granny_real32 const * ti,
                                            granny_real32 const * pi,
                                            granny_real32 t,
                                            granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline1x3n(granny_real32 const * ti,
                                             granny_real32 const * pi,
                                             granny_real32 t,
                                             granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline1x4(granny_real32 const * ti,
                                            granny_real32 const * pi,
                                            granny_real32 t,
                                            granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline1x4n(granny_real32 const * ti,
                                             granny_real32 const * pi,
                                             granny_real32 t,
                                             granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline1x9(granny_real32 const * ti,
                                            granny_real32 const * pi,
                                            granny_real32 t,
                                            granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline2x1(granny_real32 const * ti,
                                            granny_real32 const * pi,
                                            granny_real32 t,
                                            granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline2x2(granny_real32 const * ti,
                                            granny_real32 const * pi,
                                            granny_real32 t,
                                            granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline2x3(granny_real32 const * ti,
                                            granny_real32 const * pi,
                                            granny_real32 t,
                                            granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline2x3n(granny_real32 const * ti,
                                             granny_real32 const * pi,
                                             granny_real32 t,
                                             granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline2x4(granny_real32 const * ti,
                                            granny_real32 const * pi,
                                            granny_real32 t,
                                            granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline2x4n(granny_real32 const * ti,
                                             granny_real32 const * pi,
                                             granny_real32 t,
                                             granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline2x9(granny_real32 const * ti,
                                            granny_real32 const * pi,
                                            granny_real32 t,
                                            granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline3x1(granny_real32 const * ti,
                                            granny_real32 const * pi,
                                            granny_real32 t,
                                            granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline3x2(granny_real32 const * ti,
                                            granny_real32 const * pi,
                                            granny_real32 t,
                                            granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline3x3(granny_real32 const * ti,
                                            granny_real32 const * pi,
                                            granny_real32 t,
                                            granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline3x3n(granny_real32 const * ti,
                                             granny_real32 const * pi,
                                             granny_real32 t,
                                             granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline3x4(granny_real32 const * ti,
                                            granny_real32 const * pi,
                                            granny_real32 t,
                                            granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline3x4n(granny_real32 const * ti,
                                             granny_real32 const * pi,
                                             granny_real32 t,
                                             granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline3x9(granny_real32 const * ti,
                                            granny_real32 const * pi,
                                            granny_real32 t,
                                            granny_real32 * Result);
GRANNY_DYNLINK(void) GrannySampleBSpline(granny_int32x Degree,
                                         granny_int32x Dimension,
                                         bool Normalize,
                                         granny_real32 const * ti,
                                         granny_real32 const * pi,
                                         granny_real32 t,
                                         granny_real32 * Result);
GRANNY_DYNLINK(void) GrannyUncheckedSampleBSpline(granny_int32x Degree,
                                                  granny_int32x Dimension,
                                                  granny_real32 const * ti,
                                                  granny_real32 const * pi,
                                                  granny_real32 t,
                                                  granny_real32 * Result);
GRANNY_DYNLINK(void) GrannyUncheckedSampleBSplineN(granny_int32x Degree,
                                                   granny_int32x Dimension,
                                                   granny_real32 const * ti,
                                                   granny_real32 const * pi,
                                                   granny_real32 t,
                                                   granny_real32 * Result);

/* granny_bspline_buffers.h bindings: */
GRANNY_DYNLINK(bool) GrannyConstructBSplineBuffers(granny_int32x Dimension,
                                                   granny_curve2 const * PrevCurve,
                                                   granny_curve2 const * Curve,
                                                   granny_curve2 const * NextCurve,
                                                   granny_real32 PrevCurveDuration,
                                                   granny_real32 CurveDuration,
                                                   granny_real32 NextCurveDuration,
                                                   granny_int32x KnotIndex,
                                                   granny_real32 * ti,
                                                   granny_real32 * pi,
                                                   granny_real32 ** tiPtr,
                                                   granny_real32 ** piPtr,
                                                   granny_real32 const * IdentityVector);
GRANNY_DYNLINK(void) GrannyEvaluateCurveAtT(granny_int32x Dimension,
                                            bool Normalize,
                                            bool BackwardsLoop,
                                            granny_curve2 const * Curve,
                                            bool ForwardsLoop,
                                            granny_real32 CurveDuration,
                                            granny_real32 t,
                                            granny_real32 * Result,
                                            granny_real32 const * IdentityVector);
GRANNY_DYNLINK(void) GrannyEvaluateCurveAtKnotIndex(granny_int32x Dimension,
                                                    bool Normalize,
                                                    bool BackwardsLoop,
                                                    granny_curve2 const * Curve,
                                                    bool ForwardsLoop,
                                                    granny_real32 CurveDuration,
                                                    granny_int32x KnotIndex,
                                                    granny_real32 t,
                                                    granny_real32 * Result,
                                                    granny_real32 const * IdentityVector);

/* granny_bspline_solver.h bindings: */
GRANNY_DYNLINK(granny_int32x) GrannyGetMaximumKnotCountForSampleCount(granny_int32x MaxDegree,
                                                                      granny_int32x MaxSampleCount);
GRANNY_DYNLINK(granny_bspline_solver *) GrannyAllocateBSplineSolver(granny_int32x MaxDegree,
                                                                    granny_int32x MaxSampleCount,
                                                                    granny_int32x MaxDimension);
GRANNY_DYNLINK(void) GrannyDeallocateBSplineSolver(granny_bspline_solver * Solver);
GRANNY_DYNLINK(granny_curve2 * ) GrannyFitBSplineToSamples(granny_bspline_solver * Solver,
                                                           granny_uint32x SolverFlags,
                                                           granny_int32x Degree,
                                                           granny_real32 ErrorThreshold,
                                                           granny_real32 C0Threshold,
                                                           granny_real32 C1Threshold,
                                                           granny_real32 const * Samples,
                                                           granny_int32x Dimension,
                                                           granny_int32x SampleCount,
                                                           granny_real32 dT,
                                                           granny_data_type_definition const * CurveDataType,
                                                           granny_int32x MaximumCurveSizeInBytes,
                                                           bool * AchievedTolerance,
                                                           granny_int32x * CurveSizeInBytes);
GRANNY_DYNLINK(granny_real32) GrannyOrientationToleranceFromDegrees(granny_real32 Degrees);
GRANNY_DYNLINK(granny_real32) GrannyDegreesFromOrientationTolerance(granny_real32 Tolerance);
GRANNY_DYNLINK(void) GrannyGetSquaredErrorOverCurve(granny_int32x Flags,
                                                    granny_int32x Degree,
                                                    granny_int32x Dimension,
                                                    granny_real32 Duration,
                                                    granny_curve2 const * Curve,
                                                    granny_int32x SampleCount,
                                                    granny_real32 const * Samples,
                                                    granny_real32 const * IdentityVector,
                                                    granny_bspline_error * Result);

/* granny_camera.h bindings: */
GRANNY_DYNLINK(void) GrannyInitializeDefaultCamera(granny_camera * Camera);
GRANNY_DYNLINK(void) GrannySetCameraAspectRatios(granny_camera * Camera,
                                                 granny_real32 PhysicalAspectRatio,
                                                 granny_real32 ScreenWidth,
                                                 granny_real32 ScreenHeight,
                                                 granny_real32 WindowWidth,
                                                 granny_real32 WindowHeight);
GRANNY_DYNLINK(void) GrannyMoveCameraRelative(granny_camera * Camera,
                                              granny_real32 X,
                                              granny_real32 Y,
                                              granny_real32 Z);
GRANNY_DYNLINK(void) GrannyBuildCameraMatrices(granny_camera * Camera);
GRANNY_DYNLINK(void) GrannyGetCameraLocation(granny_camera const * Camera,
                                             granny_real32 * Result);
GRANNY_DYNLINK(void) GrannyGetCameraLeft(granny_camera const * Camera,
                                         granny_real32 * Result);
GRANNY_DYNLINK(void) GrannyGetCameraRight(granny_camera const * Camera,
                                          granny_real32 * Result);
GRANNY_DYNLINK(void) GrannyGetCameraUp(granny_camera const * Camera,
                                       granny_real32 * Result);
GRANNY_DYNLINK(void) GrannyGetCameraDown(granny_camera const * Camera,
                                         granny_real32 * Result);
GRANNY_DYNLINK(void) GrannyGetCameraForward(granny_camera const * Camera,
                                            granny_real32 * Result);
GRANNY_DYNLINK(void) GrannyGetCameraBack(granny_camera const * Camera,
                                         granny_real32 * Result);
GRANNY_DYNLINK(void) GrannyWindowSpaceToWorldSpace(granny_camera const * Camera,
                                                   granny_real32 Width,
                                                   granny_real32 Height,
                                                   granny_real32 const * ScreenSpacePoint,
                                                   granny_real32 * Result);
GRANNY_DYNLINK(void) GrannyWorldSpaceToWindowSpace(granny_camera const * Camera,
                                                   granny_real32 Width,
                                                   granny_real32 Height,
                                                   granny_real32 const * WorldSpacePoint,
                                                   granny_real32 * Result);
GRANNY_DYNLINK(void) GrannyGetPickingRay(granny_camera const * Camera,
                                         granny_real32 Width,
                                         granny_real32 Height,
                                         granny_real32 MouseX,
                                         granny_real32 MouseY,
                                         granny_real32 * Origin,
                                         granny_real32 * Normal);
GRANNY_DYNLINK(void) GrannyGetCameraRelativePlanarBases(granny_camera const * Camera,
                                                        bool ScreenOrthogonal,
                                                        granny_real32 const * PlaneNormal,
                                                        granny_real32 const * PointOnPlane,
                                                        granny_real32 * XBasis,
                                                        granny_real32 * YBasis);
GRANNY_DYNLINK(granny_real32) GrannyGetMostLikelyPhysicalAspectRatio(granny_int32x ScreenWidth,
                                                                     granny_int32x ScreenHeight);
GRANNY_DYNLINK(granny_real32) GrannyFindAllowedLODError(granny_real32 ErrorInPixels,
                                                        granny_int32x ViewportHeightInPixels,
                                                        granny_real32 CameraFOVY,
                                                        granny_real32 DistanceFromCamera);

/* granny_compress_curve.h bindings: */
GRANNY_DYNLINK(granny_curve2 *) GrannyCompressCurve(granny_bspline_solver * Solver,
                                                    granny_uint32x SolverFlags,
                                                    granny_compress_curve_parameters const * Params,
                                                    granny_real32 const * Samples,
                                                    granny_int32x Dimension,
                                                    granny_int32x FrameCount,
                                                    granny_real32 dT,
                                                    bool * CurveAcheivedTolerance);

/* granny_control.h bindings: */
GRANNY_DYNLINK(granny_control *) GrannyCreateControl(granny_real32 CurrentClock,
                                                     granny_real32 LocalDuration);
GRANNY_DYNLINK(void) GrannyFreeControl(granny_control * Control);
GRANNY_DYNLINK(void) GrannyFreeControlOnceUnused(granny_control * Control);
GRANNY_DYNLINK(void) GrannyCompleteControlAt(granny_control * Control,
                                             granny_real32 AtSeconds);
GRANNY_DYNLINK(granny_real32) GrannyGetControlCompletionClock(granny_control const * Control);
GRANNY_DYNLINK(bool) GrannyGetControlCompletionCheckFlag(granny_control const * Control);
GRANNY_DYNLINK(void) GrannySetControlCompletionCheckFlag(granny_control * Control,
                                                         bool CheckComplete);
GRANNY_DYNLINK(granny_real32) GrannyGetControlClock(granny_control const * Control);
GRANNY_DYNLINK(void) GrannySetControlClock(granny_control * Control,
                                           granny_real32 NewSeconds);
GRANNY_DYNLINK(void) GrannySetControlClockOnly(granny_control * Control,
                                               granny_real32 NewSeconds);
GRANNY_DYNLINK(bool) GrannyControlIsComplete(granny_control const * Control);
GRANNY_DYNLINK(bool) GrannyFreeControlIfComplete(granny_control * Control);
GRANNY_DYNLINK(granny_real32) GrannyGetControlWeight(granny_control const * Control);
GRANNY_DYNLINK(void) GrannySetControlWeight(granny_control * Control,
                                            granny_real32 Weight);
GRANNY_DYNLINK(granny_track_mask const* ) GrannyGetControlTrackGroupModelMask(granny_control const * Control,
                                                                              granny_model_instance const*  ModelInstance);
GRANNY_DYNLINK(granny_track_mask const* ) GrannyGetControlTrackGroupTrackMask(granny_control const * Control,
                                                                              granny_animation const*  Animation,
                                                                              granny_int32x TrackGroupIndex);
GRANNY_DYNLINK(granny_int32x) GrannyGetControlLoopCount(granny_control const * Control);
GRANNY_DYNLINK(void) GrannySetControlLoopCount(granny_control * Control,
                                               granny_int32x LoopCount);
GRANNY_DYNLINK(void) GrannyGetControlLoopState(granny_control * Control,
                                               bool * UnderflowLoop,
                                               bool * OverflowLoop);
GRANNY_DYNLINK(granny_int32x) GrannyGetControlLoopIndex(granny_control * Control);
GRANNY_DYNLINK(void) GrannySetControlLoopIndex(granny_control * Control,
                                               granny_int32x LoopIndex);
GRANNY_DYNLINK(granny_real32) GrannyGetControlSpeed(granny_control const * Control);
GRANNY_DYNLINK(void) GrannySetControlSpeed(granny_control * Control,
                                           granny_real32 Speed);
GRANNY_DYNLINK(granny_real32) GrannyGetControlDuration(granny_control const * Control);
GRANNY_DYNLINK(granny_real32) GrannyGetControlDurationLeft(granny_control * Control);
GRANNY_DYNLINK(bool) GrannyControlIsActive(granny_control const * Control);
GRANNY_DYNLINK(void) GrannySetControlActive(granny_control * Control,
                                            bool Active);
GRANNY_DYNLINK(granny_real32) GrannyGetControlClampedLocalClock(granny_control * Control);
GRANNY_DYNLINK(granny_real32) GrannyGetControlLocalDuration(granny_control const * Control);
GRANNY_DYNLINK(granny_real32) GrannyGetControlEaseCurveMultiplier(granny_control const * Control);
GRANNY_DYNLINK(granny_real32) GrannyGetControlEffectiveWeight(granny_control const * Control);
GRANNY_DYNLINK(void) GrannySetControlEaseIn(granny_control * Control,
                                            bool EaseIn);
GRANNY_DYNLINK(void) GrannySetControlEaseInCurve(granny_control * Control,
                                                 granny_real32 StartSeconds,
                                                 granny_real32 EndSeconds,
                                                 granny_real32 StartValue,
                                                 granny_real32 StartTangent,
                                                 granny_real32 EndTangent,
                                                 granny_real32 EndValue);
GRANNY_DYNLINK(void) GrannySetControlEaseOut(granny_control * Control,
                                             bool EaseOut);
GRANNY_DYNLINK(void) GrannySetControlEaseOutCurve(granny_control * Control,
                                                  granny_real32 StartSeconds,
                                                  granny_real32 EndSeconds,
                                                  granny_real32 StartValue,
                                                  granny_real32 StartTangent,
                                                  granny_real32 EndTangent,
                                                  granny_real32 EndValue);
GRANNY_DYNLINK(granny_real32) GrannyGetControlRawLocalClock(granny_control * Control);
GRANNY_DYNLINK(void) GrannySetControlRawLocalClock(granny_control * Control,
                                                   granny_real32 LocalClock);
GRANNY_DYNLINK(granny_real32) GrannyEaseControlIn(granny_control * Control,
                                                  granny_real32 Duration,
                                                  bool FromCurrent);
GRANNY_DYNLINK(granny_real32) GrannyEaseControlOut(granny_control * Control,
                                                   granny_real32 Duration);
GRANNY_DYNLINK(void **) GrannyGetControlUserDataArray(granny_control const * Control);
GRANNY_DYNLINK(granny_control *) GrannyGetGlobalControlsBegin(void);
GRANNY_DYNLINK(granny_control *) GrannyGetGlobalControlsEnd(void);
GRANNY_DYNLINK(granny_control *) GrannyGetGlobalNextControl(granny_control * Control);
GRANNY_DYNLINK(void) GrannyRecenterControlClocks(granny_control * Control,
                                                 granny_real32 dCurrentClock);
GRANNY_DYNLINK(void) GrannyRecenterAllControlClocks(granny_real32 dCurrentClock);
GRANNY_DYNLINK(void) GrannyRecenterAllModelInstanceControlClocks(granny_model_instance * ModelInstance,
                                                                 granny_real32 dCurrentClock);
GRANNY_DYNLINK(void) GrannySetControlForceClampedLooping(granny_control * Control,
                                                         bool Clamp);
GRANNY_DYNLINK(void) GrannySetControlTargetState(granny_control * Control,
                                                 granny_real32 CurrentGlobalTime,
                                                 granny_real32 TargetGlobalTime,
                                                 granny_real32 LocalTime,
                                                 granny_int32x LoopIndex);

/* granny_controlled_animation.h bindings: */
GRANNY_DYNLINK(granny_control *) GrannyPlayControlledAnimation(granny_real32 StartTime,
                                                               granny_animation const * Animation,
                                                               granny_model_instance * Model);
GRANNY_DYNLINK(granny_control *) GrannyPlayControlledAnimationBinding(granny_real32 StartTime,
                                                                      granny_animation const * Animation,
                                                                      granny_animation_binding * Binding,
                                                                      granny_model_instance * Model);
GRANNY_DYNLINK(granny_controlled_animation_builder *) GrannyBeginControlledAnimation(granny_real32 StartTime,
                                                                                     granny_animation const * Animation);
GRANNY_DYNLINK(granny_control *) GrannyEndControlledAnimation(granny_controlled_animation_builder * Builder);
GRANNY_DYNLINK(void) GrannyUseExistingControlForAnimation(granny_controlled_animation_builder * Builder,
                                                          granny_control * Control);
GRANNY_DYNLINK(void) GrannySetTrackMatchRule(granny_controlled_animation_builder * Builder,
                                             granny_int32x TrackGroupIndex,
                                             char const * TrackPattern,
                                             char const * BonePattern);
GRANNY_DYNLINK(void) GrannySetTrackGroupTarget(granny_controlled_animation_builder * Builder,
                                               granny_int32x TrackGroupIndex,
                                               granny_model_instance * Model);
GRANNY_DYNLINK(void) GrannySetTrackGroupBinding(granny_controlled_animation_builder * Builder,
                                                granny_int32x TrackGroupIndex,
                                                granny_animation_binding * Binding);
GRANNY_DYNLINK(void) GrannySetTrackGroupBasisTransform(granny_controlled_animation_builder * Builder,
                                                       granny_int32x TrackGroupIndex,
                                                       granny_model * FromModel,
                                                       granny_model * ToModel);
GRANNY_DYNLINK(void) GrannySetTrackGroupTrackMask(granny_controlled_animation_builder * Builder,
                                                  granny_int32x TrackGroupIndex,
                                                  granny_track_mask * TrackMask);
GRANNY_DYNLINK(void) GrannySetTrackGroupModelMask(granny_controlled_animation_builder * Builder,
                                                  granny_int32x TrackGroupIndex,
                                                  granny_track_mask * ModelMask);
GRANNY_DYNLINK(void) GrannySetTrackGroupAccumulation(granny_controlled_animation_builder * Builder,
                                                     granny_int32x TrackGroupIndex,
                                                     granny_accumulation_mode Mode);
GRANNY_DYNLINK(void) GrannySetTrackGroupLOD(granny_controlled_animation_builder * Builder,
                                            granny_int32x TrackGroupIndex,
                                            bool CopyValuesFromAnimation,
                                            granny_real32 ManualScaler);
GRANNY_DYNLINK(granny_animation_binding *) GrannyGetAnimationBindingFromControlBinding(granny_model_control_binding * Binding);
GRANNY_DYNLINK(granny_real32) GrannyGetGlobalLODFadingFactor(void);
GRANNY_DYNLINK(void) GrannySetGlobalLODFadingFactor(granny_real32 NewValue);

/* granny_controlled_pose.h bindings: */
GRANNY_DYNLINK(granny_control *) GrannyPlayControlledPose(granny_real32 StartTime,
                                                          granny_real32 Duration,
                                                          granny_local_pose const * Pose,
                                                          granny_model_instance * Model,
                                                          granny_track_mask * ModelMask);
GRANNY_DYNLINK(granny_local_pose *) GrannyGetLocalPoseFromControlBinding(granny_model_control_binding * Binding);

/* granny_crc.h bindings: */
GRANNY_DYNLINK(void) GrannyBeginCRC32(granny_uint32 * CRC);
GRANNY_DYNLINK(void) GrannyAddToCRC32(granny_uint32 * CRC,
                                      granny_uint32 Count,
                                      void const * UInt8s);
GRANNY_DYNLINK(void) GrannyEndCRC32(granny_uint32 * CRC);

/* granny_dag.h bindings: */
GRANNY_DYNLINK(bool) GrannyIsBlendDagLeafType(granny_blend_dag_node const * Node);
GRANNY_DYNLINK(granny_blend_dag_node_type) GrannyGetBlendDagNodeType(granny_blend_dag_node const * Node);
GRANNY_DYNLINK(granny_blend_dag_node *) GrannyCreateBlendDagNodeAnimationBlend(granny_model_instance * ModelInstance,
                                                                               bool AutoFree);
GRANNY_DYNLINK(granny_blend_dag_node *) GrannyCreateBlendDagNodeLocalPose(granny_local_pose * LocalPose,
                                                                          bool AutoFree);
GRANNY_DYNLINK(granny_blend_dag_node *) GrannyCreateBlendDagNodeCallback(granny_blend_dag_leaf_callback_sample_callback * SampleCallback,
                                                                         granny_blend_dag_leaf_callback_set_clock_callback * SetClockCallback,
                                                                         granny_blend_dag_leaf_callback_motion_vectors_callback * MotionVectorsCallback,
                                                                         void * UserData);
GRANNY_DYNLINK(granny_blend_dag_node *) GrannyCreateBlendDagNodeCrossfade(granny_blend_dag_node * DagNode0,
                                                                          granny_blend_dag_node * DagNode1,
                                                                          granny_real32 WeightNone,
                                                                          granny_real32 WeightAll,
                                                                          granny_track_mask * TrackMask,
                                                                          bool AutoFreeTrackMask,
                                                                          granny_int32x BoneCount);
GRANNY_DYNLINK(granny_blend_dag_node *) GrannyCreateBlendDagNodeWeightedBlend(granny_skeleton * ReferenceSkeleton,
                                                                              granny_real32 FillThreshold,
                                                                              granny_quaternion_mode QuaternionBlendingMode,
                                                                              granny_int32x BoneCount);
GRANNY_DYNLINK(void) GrannyFreeBlendDagNode(granny_blend_dag_node * Node);
GRANNY_DYNLINK(void) GrannySetBlendDagNodeAnimationBlend(granny_blend_dag_node * Node,
                                                         granny_model_instance * ModelInstance,
                                                         granny_real32 FillThreshold,
                                                         bool AutoFree);
GRANNY_DYNLINK(void) GrannySetBlendDagNodeLocalPose(granny_blend_dag_node * Node,
                                                    granny_local_pose * LocalPose,
                                                    bool AutoFree);
GRANNY_DYNLINK(void) GrannySetBlendDagNodeCallbacks(granny_blend_dag_node * Node,
                                                    granny_blend_dag_leaf_callback_sample_callback * SampleCallback,
                                                    granny_blend_dag_leaf_callback_set_clock_callback * SetClockCallback,
                                                    granny_blend_dag_leaf_callback_motion_vectors_callback * MotionVectorsCallback,
                                                    void * UserData);
GRANNY_DYNLINK(void) GrannySetBlendDagNodeCrossfade(granny_blend_dag_node * Node,
                                                    granny_real32 WeightNone,
                                                    granny_real32 WeightAll,
                                                    granny_track_mask * TrackMask,
                                                    bool AutoFreeTrackMask);
GRANNY_DYNLINK(void) GrannySetBlendDagNodeCrossfadeWeights(granny_blend_dag_node * Node,
                                                           granny_real32 WeightNone,
                                                           granny_real32 WeightAll);
GRANNY_DYNLINK(void) GrannySetBlendDagNodeWeightedBlend(granny_blend_dag_node * Node,
                                                        granny_skeleton * ReferenceSkeleton,
                                                        granny_real32 FillThreshold,
                                                        granny_quaternion_mode QuaternionBlendingMode);
GRANNY_DYNLINK(granny_model_instance *) GrannyGetBlendDagNodeAnimationBlend(granny_blend_dag_node * Node);
GRANNY_DYNLINK(granny_local_pose *) GrannyGetBlendDagNodeLocalPose(granny_blend_dag_node * Node);
GRANNY_DYNLINK(granny_blend_dag_leaf_callback_sample_callback *) GrannyGetBlendDagNodeCallbackSampleCallback(granny_blend_dag_node * Node);
GRANNY_DYNLINK(granny_blend_dag_leaf_callback_set_clock_callback *) GrannyGetBlendDagNodeCallbackSetClockCallback(granny_blend_dag_node * Node);
GRANNY_DYNLINK(granny_blend_dag_leaf_callback_motion_vectors_callback *) GrannyGetBlendDagNodeCallbackMotionVectorsCallback(granny_blend_dag_node * Node);
GRANNY_DYNLINK(void *) GrannyGetBlendDagNodeCallbackUserData(granny_blend_dag_node * Node);
GRANNY_DYNLINK(granny_track_mask *) GrannyGetBlendDagNodeCrossfadeTrackMask(granny_blend_dag_node * Node);
GRANNY_DYNLINK(bool) GrannyGetBlendDagNodeCrossfadeWeights(granny_blend_dag_node const * Node,
                                                           granny_real32 *  WeightNone,
                                                           granny_real32 *  WeightAll);
GRANNY_DYNLINK(granny_skeleton *) GrannyGetBlendDagNodeWeightedBlendSkeleton(granny_blend_dag_node * Node);
GRANNY_DYNLINK(void) GrannyBlendDagNodeAnimationBlendFreeCompletedControls(granny_blend_dag_node * Node);
GRANNY_DYNLINK(void) GrannyClearBlendDagNodeChildren(granny_blend_dag_node * Node);
GRANNY_DYNLINK(granny_int32x) GrannyGetBlendDagNodeChildrenCount(granny_blend_dag_node const * Node);
GRANNY_DYNLINK(void) GrannySetBlendDagNodeChildren(granny_blend_dag_node * Node,
                                                   granny_int32x NumChildren,
                                                   granny_blend_dag_node ** ArrayOfChildren);
GRANNY_DYNLINK(void) GrannySetBlendDagNodeChild(granny_blend_dag_node * Node,
                                                granny_int32x ChildNumber,
                                                granny_blend_dag_node * Child);
GRANNY_DYNLINK(granny_blend_dag_node *) GrannyGetBlendDagNodeChild(granny_blend_dag_node * Node,
                                                                   granny_int32x ChildNumber);
GRANNY_DYNLINK(void) GrannyAddBlendDagNodeChild(granny_blend_dag_node * Node,
                                                granny_blend_dag_node * Child);
GRANNY_DYNLINK(void) GrannyRemoveBlendDagNodeChild(granny_blend_dag_node * Node,
                                                   granny_blend_dag_node * Child);
GRANNY_DYNLINK(granny_blend_dag_node *) GrannyGetBlendDagNodeParent(granny_blend_dag_node * Node);
GRANNY_DYNLINK(void) GrannySetBlendDagNodeResultTrackMask(granny_blend_dag_node * Node,
                                                          granny_track_mask * TrackMask,
                                                          bool AutoFree);
GRANNY_DYNLINK(granny_track_mask *) GrannyGetBlendDagNodeResultTrackMask(granny_blend_dag_node * Node);
GRANNY_DYNLINK(void) GrannySetBlendDagNodeResultWeight(granny_blend_dag_node * Node,
                                                       granny_real32 Weight);
GRANNY_DYNLINK(granny_real32) GrannyGetBlendDagNodeResultWeight(granny_blend_dag_node const * Node);
GRANNY_DYNLINK(void) GrannyGetBlendDagTreeMotionVectors(granny_blend_dag_node const * Node,
                                                        granny_real32 SecondsElapsed,
                                                        granny_real32 * ResultTranslation,
                                                        granny_real32 * ResultRotation,
                                                        bool Inverse);
GRANNY_DYNLINK(void) GrannyUpdateBlendDagTreeMatrix(granny_blend_dag_node const * Node,
                                                    granny_real32 SecondsElapsed,
                                                    granny_real32 const * ModelMatrix4x4,
                                                    granny_real32 * DestMatrix4x4,
                                                    bool Inverse);
GRANNY_DYNLINK(void) GrannyPrimeBlendDagLocalPoseCache(granny_int32x MaxNumBones,
                                                       granny_int32x MaxTreeDepth);
GRANNY_DYNLINK(void) GrannyFreeBlendDagLocalPoseCache(void);
GRANNY_DYNLINK(granny_dag_pose_cache *) GrannyCreateDagPoseCache(granny_int32x InitNumBones,
                                                                 granny_int32x InitTreeDepth);
GRANNY_DYNLINK(void) GrannyFreeDagPoseCache(granny_dag_pose_cache * Cache);
GRANNY_DYNLINK(granny_local_pose *) GrannySampleBlendDagTreeLODSparseReentrant(granny_blend_dag_node const * RootNode,
                                                                               granny_int32x BoneCount,
                                                                               granny_real32 AllowedError,
                                                                               granny_int32x const * SparseBoneArray,
                                                                               granny_dag_pose_cache * PoseCache);
GRANNY_DYNLINK(granny_local_pose *) GrannySampleBlendDagTreeLODSparse(granny_blend_dag_node const * RootNode,
                                                                      granny_int32x BoneCount,
                                                                      granny_real32 AllowedError,
                                                                      granny_int32x const * SparseBoneArray);
GRANNY_DYNLINK(granny_local_pose *) GrannySampleBlendDagTree(granny_blend_dag_node const * RootNode,
                                                             granny_int32x BoneCount);
GRANNY_DYNLINK(granny_local_pose *) GrannySampleBlendDagTreeLOD(granny_blend_dag_node const * RootNode,
                                                                granny_int32x BoneCount,
                                                                granny_real32 AllowedError);
GRANNY_DYNLINK(granny_local_pose *) GrannySampleBlendDagTreeReentrant(granny_blend_dag_node const * RootNode,
                                                                      granny_int32x BoneCount,
                                                                      granny_dag_pose_cache * PoseCache);
GRANNY_DYNLINK(granny_local_pose *) GrannySampleBlendDagTreeLODReentrant(granny_blend_dag_node const * RootNode,
                                                                         granny_int32x BoneCount,
                                                                         granny_real32 AllowedError,
                                                                         granny_dag_pose_cache * PoseCache);
GRANNY_DYNLINK(void) GrannySetBlendDagTreeClock(granny_blend_dag_node * RootNode,
                                                granny_real32 NewClock);
GRANNY_DYNLINK(void) GrannyGetBlendDagNodeChildren(granny_blend_dag_node * Node,
                                                   granny_int32x MaxArraySize,
                                                   granny_blend_dag_node ** ArrayOfChildren);
GRANNY_DYNLINK(granny_blend_dag_node *) GrannyCreateBlendDagNodeWeightedBlendChildren(granny_skeleton * ReferenceSkeleton,
                                                                                      granny_real32 FillThreshold,
                                                                                      granny_quaternion_mode QuaternionBlendingMode,
                                                                                      granny_int32x BoneCount,
                                                                                      granny_int32x NumChildren,
                                                                                      granny_blend_dag_node ** ArrayOfChildren);
GRANNY_DYNLINK(void) GrannyFreeBlendDagEntireTree(granny_blend_dag_node * RootNode);
GRANNY_DYNLINK(void) GrannyBlendDagFreeCompletedControlsEntireTree(granny_blend_dag_node * Node);
GRANNY_DYNLINK(granny_blend_dag_node *) GrannyDuplicateBlendDagTree(granny_blend_dag_node const * SourceTreeRoot,
                                                                    granny_blend_dag_node const ** SourceNodeList,
                                                                    granny_blend_dag_node ** DestNodeList,
                                                                    granny_int32 SizeOfNodeList,
                                                                    bool AutoFreeCreatedModelInstances);
GRANNY_DYNLINK(bool) GrannyIsBlendDagNodeValid(granny_blend_dag_node const * Node,
                                               char ** ReasonFailed,
                                               granny_blend_dag_node const ** NodeFailed);
GRANNY_DYNLINK(bool) GrannyIsBlendDagTreeValid(granny_blend_dag_node const * RootNode,
                                               char ** ReasonFailed,
                                               granny_blend_dag_node const ** NodeFailed);
GRANNY_DYNLINK(granny_int32x) GrannyFindBlendDagTreeDepth(granny_blend_dag_node const * RootNode);

/* granny_data_type_conversion.h bindings: */
GRANNY_DYNLINK(bool) GrannyFindMatchingMember(granny_data_type_definition const * SourceType,
                                              void const * SourceObject,
                                              char const * DestMemberName,
                                              granny_variant *  Result);
GRANNY_DYNLINK(void) GrannyConvertSingleObject(granny_data_type_definition const * SourceType,
                                               void const * SourceObject,
                                               granny_data_type_definition const * DestType,
                                               void * DestObject);
GRANNY_DYNLINK(void) GrannyMergeSingleObject(granny_data_type_definition const * SourceType,
                                             void const * SourceObject,
                                             granny_data_type_definition const * DestType,
                                             void * DestObject);
GRANNY_DYNLINK(void *) GrannyConvertTree(granny_data_type_definition const * SourceType,
                                         void const * SourceTree,
                                         granny_data_type_definition const * DestType);
GRANNY_DYNLINK(granny_int32x) GrannyGetConvertedTreeSize(granny_data_type_definition const * SourceType,
                                                         void const * SourceTree,
                                                         granny_data_type_definition const * DestType);
GRANNY_DYNLINK(void *) GrannyConvertTreeInPlace(granny_data_type_definition const * SourceType,
                                                void const * SourceTree,
                                                granny_data_type_definition const * DestType,
                                                void * Memory);
GRANNY_DYNLINK(bool) GrannyRebasePointers(granny_data_type_definition const * Type,
                                          void * Data,
                                          granny_intaddrx Offset,
                                          bool RebaseStrings);
GRANNY_DYNLINK(bool) GrannyRebasePointersStringCallback(granny_data_type_definition const * Type,
                                                        void * Data,
                                                        granny_intaddrx Offset,
                                                        granny_rebase_pointers_string_callback * Callback,
                                                        void * CallbackData);

/* granny_data_type_io.h bindings: */
GRANNY_DYNLINK(granny_file_data_tree_writer *) GrannyBeginFileDataTreeWriting(granny_data_type_definition const * RootObjectTypeDefinition,
                                                                              void *  RootObject,
                                                                              granny_int32x DefaultTypeSectionIndex,
                                                                              granny_int32x DefaultObjectSectionIndex);
GRANNY_DYNLINK(void) GrannyPreserveObjectFileSections(granny_file_data_tree_writer *  Writer,
                                                      granny_file const * SourceFile);
GRANNY_DYNLINK(void) GrannyEndFileDataTreeWriting(granny_file_data_tree_writer * Writer);
GRANNY_DYNLINK(void) GrannySetFileDataTreeFlags(granny_file_data_tree_writer * Writer,
                                                granny_uint32x Flags);
GRANNY_DYNLINK(void) GrannySetFileSectionForObjectsOfType(granny_file_data_tree_writer * Writer,
                                                          granny_data_type_definition const * Type,
                                                          granny_int32x SectionIndex);
GRANNY_DYNLINK(void) GrannySetFileSectionForObject(granny_file_data_tree_writer * Writer,
                                                   void const * Object,
                                                   granny_int32x SectionIndex);
GRANNY_DYNLINK(bool) GrannyWriteDataTreeToFileBuilder(granny_file_data_tree_writer * Writer,
                                                      granny_file_builder * Builder);
GRANNY_DYNLINK(bool) GrannyWriteDataTreeToFile(granny_file_data_tree_writer * Writer,
                                               granny_uint32 FileTypeTag,
                                               granny_grn_file_magic_value const * PlatformMagicValue,
                                               char const * FileName,
                                               granny_int32x FileSectionCount);
GRANNY_DYNLINK(void) GrannySetFileWriterStringCallback(granny_file_data_tree_writer * Writer,
                                                       granny_file_writer_string_callback * Callback,
                                                       void * Data);

/* granny_degree_of_freedom.h bindings: */
GRANNY_DYNLINK(bool) GrannyClipPositionDOFs(granny_real32 * Position,
                                            granny_uint32x AllowedDOFs);
GRANNY_DYNLINK(bool) GrannyClipAngularVelocityDOFs(granny_real32 * Orientation,
                                                   granny_uint32x AllowedDOFs);
GRANNY_DYNLINK(bool) GrannyClipOrientationDOFs(granny_real32 * Orientation,
                                               granny_uint32x AllowedDOFs);
GRANNY_DYNLINK(void) GrannyClipTransformDOFs(granny_transform * Result,
                                             granny_uint32x AllowedDOFs);

/* granny_exporter_info.h bindings: */
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyExporterInfoType;

/* granny_file.h bindings: */
GRANNY_DYNLINK(bool) GrannyFileCRCIsValid(char const * FileName);
GRANNY_DYNLINK(granny_file *) GrannyReadEntireFile(char const * FileName);
GRANNY_DYNLINK(granny_file *) GrannyReadEntireFileFromMemory(granny_int32x MemorySize,
                                                             void * Memory);
GRANNY_DYNLINK(granny_file *) GrannyReadEntireFileFromReader(granny_file_reader * Reader);
GRANNY_DYNLINK(granny_file *) GrannyReadPartialFileFromReader(granny_file_reader * Reader);
GRANNY_DYNLINK(void) GrannyFreeFileSection(granny_file * File,
                                           granny_int32x SectionIndex);
GRANNY_DYNLINK(void) GrannyFreeAllFileSections(granny_file * File);
GRANNY_DYNLINK(bool) GrannyReadFileSection(granny_file_reader * Reader,
                                           granny_file * File,
                                           granny_int32x SectionIndex);
GRANNY_DYNLINK(bool) GrannyReadFileSectionInPlace(granny_file_reader * Reader,
                                                  granny_file * File,
                                                  granny_int32x SectionIndex,
                                                  void * Memory);
GRANNY_DYNLINK(void) GrannyFixupFileSectionPhase1(granny_file * File,
                                                  granny_int32x SectionIndex,
                                                  granny_grn_pointer_fixup const*  PointerFixupArray);
GRANNY_DYNLINK(bool) GrannyFixupFileSectionPhase2(granny_file_reader * Reader,
                                                  granny_file * File,
                                                  granny_int32x SectionIndex,
                                                  granny_grn_pointer_fixup const*  PointerFixupArray);
GRANNY_DYNLINK(bool) GrannyLoadFixupArray(granny_file_reader * Reader,
                                          granny_grn_section const * Section,
                                          bool FileIsByteReversed,
                                          granny_grn_pointer_fixup **  Array);
GRANNY_DYNLINK(void) GrannyFreeFile(granny_file * File);
GRANNY_DYNLINK(granny_int32x) GrannyGetFileSectionOfLoadedObject(granny_file const * File,
                                                                 void const * Object);
GRANNY_DYNLINK(void) GrannyGetDataTreeFromFile(granny_file const * File,
                                               granny_variant *  Result);
GRANNY_DYNLINK(granny_uint32) GrannyGetFileTypeTag(granny_file const * File);
GRANNY_DYNLINK(bool) GrannyRecompressFile(char const * SourceFile,
                                          char const * DestFile,
                                          granny_int32x CompressorMappingCount,
                                          granny_int32x const * CompressorMapping);
GRANNY_DYNLINK(bool) GrannyConvertFileInfoToRaw(granny_file_info *  SourceFileInfo,
                                                char const * DestFileName);
GRANNY_DYNLINK(bool) GrannyConvertFileToRaw(char const * SourceFileName,
                                            char const * DestFileName);
GRANNY_DYNLINK(bool) GrannyPlatformConvertReaderToWriter(granny_file_reader * SourceReader,
                                                         granny_file_writer * DestWriter,
                                                         granny_grn_file_magic_value const * DestMagicValue,
                                                         bool ExcludeTypeTree);
GRANNY_DYNLINK(granny_uint32) GrannyGetInMemoryFileCRC(granny_file * File);

/* granny_file_builder.h bindings: */
GRANNY_DYNLINK(granny_file_builder *) GrannyBeginFile(granny_int32x SectionCount,
                                                      granny_uint32 FileTypeTag,
                                                      granny_grn_file_magic_value const * PlatformMagicValue,
                                                      char const * TemporaryDirectory,
                                                      char const * TemporaryFileNameRoot);
GRANNY_DYNLINK(granny_file_builder *) GrannyBeginFileInMemory(granny_int32x SectionCount,
                                                              granny_uint32 FileTypeTag,
                                                              granny_grn_file_magic_value const * PlatformMagicValue,
                                                              granny_int32x MemoryBlockSize);
GRANNY_DYNLINK(bool) GrannyEndFile(granny_file_builder * Builder,
                                   char const * FileName);
GRANNY_DYNLINK(bool) GrannyEndFileToWriter(granny_file_builder * Builder,
                                           granny_file_writer * ToFile);
GRANNY_DYNLINK(bool) GrannyEndFileRaw(granny_file_builder * Builder,
                                      char const * FileName);
GRANNY_DYNLINK(bool) GrannyEndFileRawToWriter(granny_file_builder * Builder,
                                              granny_file_writer * ToFile);
GRANNY_DYNLINK(void) GrannyAbortFile(granny_file_builder * Builder);
GRANNY_DYNLINK(void) GrannySetFileSectionFormat(granny_file_builder * Builder,
                                                granny_int32x SectionIndex,
                                                granny_compression_type Compression,
                                                granny_int32x Alignment);
GRANNY_DYNLINK(void) GrannyPreserveFileSectionFormats(granny_file_builder * Builder,
                                                      granny_file const * SourceFile);
GRANNY_DYNLINK(void) GrannyWriteFileChunk(granny_file_builder * Builder,
                                          granny_int32x InSectionIndex,
                                          granny_uint32 Marshalling,
                                          granny_uint32x Size,
                                          void const * Data,
                                          granny_file_location *  Result);
GRANNY_DYNLINK(void) GrannyOffsetFileLocation(granny_file_builder * Builder,
                                              granny_file_location const * Location,
                                              granny_uint32 AdditionalOffset,
                                              granny_file_location *  Result);
GRANNY_DYNLINK(granny_file_fixup *) GrannyMarkFileFixup(granny_file_builder * Builder,
                                                        granny_file_location const * From,
                                                        granny_int32x FromOffset,
                                                        granny_file_location const * To);
GRANNY_DYNLINK(void) GrannyAdjustFileFixup(granny_file_builder * Builder,
                                           granny_file_fixup * Fixup,
                                           granny_file_location const * NewTo);
GRANNY_DYNLINK(void) GrannyMarkMarshallingFixup(granny_file_builder * Builder,
                                                granny_file_location const * Type,
                                                granny_file_location const * Object,
                                                granny_int32x ArrayCount);
GRANNY_DYNLINK(void) GrannyMarkFileRootObject(granny_file_builder * Builder,
                                              granny_file_location const * TypeLocation,
                                              granny_file_location const * ObjectLocation);
GRANNY_DYNLINK(void) GrannySetFileStringDatabaseCRC(granny_file_builder * Builder,
                                                    granny_uint32 DatabaseCRC);
GRANNY_DYNLINK(void) GrannySetFileExtraTag(granny_file_builder * Builder,
                                           granny_int32x Index,
                                           granny_uint32 Value);

/* granny_file_compressor.h bindings: */
GRANNY_DYNLINK(granny_int32x) GrannyGetCompressedBytesPaddingSize(granny_compression_type Format);
GRANNY_DYNLINK(bool) GrannyDecompressData(granny_compression_type Format,
                                          bool FileIsByteReversed,
                                          granny_int32x CompressedBytesSize,
                                          void * CompressedBytes,
                                          granny_int32x Stop0,
                                          granny_int32x Stop1,
                                          granny_int32x Stop2,
                                          void * DecompressedBytes);
GRANNY_DYNLINK(bool) GrannyDecompressDataChunk(granny_compression_type Format,
                                               bool FileIsByteReversed,
                                               granny_int32x CompressedBytesSize,
                                               void * CompressedBytes,
                                               granny_int32x DecompressedBytesSize,
                                               void * DecompressedBytes);
GRANNY_DYNLINK(granny_file_compressor *) GrannyBeginFileCompression(granny_uint32x ExpandedDataSize,
                                                                    granny_int32x ContentsCount,
                                                                    granny_compression_type Type,
                                                                    bool WritingForReversedPlatform,
                                                                    granny_file_writer * Writer);
GRANNY_DYNLINK(bool) GrannyCompressContentsOfFile(granny_file_compressor * Compressor,
                                                  granny_int32x FileSize,
                                                  char const * FileName,
                                                  granny_int32x CopyBufferSize,
                                                  void * CopyBuffer);
GRANNY_DYNLINK(bool) GrannyCompressContentsOfReader(granny_file_compressor * Compressor,
                                                    granny_int32x FileSize,
                                                    granny_file_reader * Reader,
                                                    granny_int32x CopyBufferSize,
                                                    void * CopyBuffer);
GRANNY_DYNLINK(bool) GrannyCompressContentsOfMemory(granny_file_compressor * Compressor,
                                                    granny_int32x BufferSize,
                                                    void const * Buffer);
GRANNY_DYNLINK(bool) GrannyEndFileCompression(granny_file_compressor * Compressor,
                                              granny_uint32x * CompressedSize);

/* granny_file_format.h bindings: */
GRANNY_DYNLINK(char const *) GrannyGetStandardSectionName(granny_int32x SectionIndex);
GRANNY_DYNLINK(bool) GrannyIsGrannyFile(granny_grn_file_magic_value const * MagicValue,
                                        granny_uint32x * TotalHeaderSize,
                                        bool * IsByteReversed,
                                        bool * IsPointerSizeDifferent);
GRANNY_DYNLINK(granny_grn_section *) GrannyGetGRNSectionArray(granny_grn_file_header const * Header);
GRANNY_DYNLINK(void) GrannyReverseSection(granny_int32x First16Bit,
                                          granny_int32x First8Bit,
                                          granny_int32x End,
                                          void * BufferInit);
GRANNY_DYNLINK(void) GrannyGRNFixUp(granny_uint32x FixupCount,
                                    granny_grn_pointer_fixup const * FixupArray,
                                    void const ** ToSections,
                                    void * FromSection);
GRANNY_DYNLINK(void) GrannyGRNMarshall(granny_uint32x FixupCount,
                                       granny_grn_mixed_marshalling_fixup const * FixupArray,
                                       void const ** ToSections,
                                       void * FromSection);
GRANNY_DYNLINK(void *) GrannyDecodeGRNReference(void const ** Sections,
                                                granny_grn_reference const * Reference);
GRANNY_DYNLINK(bool) GrannyGetMagicValueForPlatform(granny_int32x PointerSizeInBits,
                                                    bool LittleEndian,
                                                    granny_grn_file_magic_value * MagicValue);
GRANNY_DYNLINK(void) GrannyGetThisPlatformProperties(granny_int32x *  PointerSize,
                                                     bool *  LittleEndian);
GRANNY_DYNLINK(bool) GrannyGetMagicValueProperties(granny_grn_file_magic_value const*  MagicValue,
                                                   granny_int32x *  PointerSize,
                                                   bool *    LittleEndian);
extern GRANNY_DYNLINKDATA(granny_grn_file_magic_value const  *) GrannyGRNFileMV_Old;
extern GRANNY_DYNLINKDATA(granny_grn_file_magic_value const  *) GrannyGRNFileMV_32Bit_LittleEndian;
extern GRANNY_DYNLINKDATA(granny_grn_file_magic_value const  *) GrannyGRNFileMV_32Bit_BigEndian;
extern GRANNY_DYNLINKDATA(granny_grn_file_magic_value const  *) GrannyGRNFileMV_64Bit_LittleEndian;
extern GRANNY_DYNLINKDATA(granny_grn_file_magic_value const  *) GrannyGRNFileMV_64Bit_BigEndian;
extern GRANNY_DYNLINKDATA(granny_grn_file_magic_value const  *) GrannyGRNFileMV_ThisPlatform;

/* granny_file_info.h bindings: */
GRANNY_DYNLINK(granny_file_info *) GrannyGetFileInfo(granny_file * File);
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyFileInfoType;

/* granny_file_operations.h bindings: */
GRANNY_DYNLINK(char const *) GrannyGetTemporaryDirectory(void);

/* granny_file_reader.h bindings: */
GRANNY_DYNLINK(void) GrannyInitializeFileReader(char const * SourceFileName,
                                                granny_int32x SourceLineNumber,
                                                granny_close_file_reader_callback * CloseFileReaderCallback,
                                                granny_read_at_most_callback * ReadAtMostCallback,
                                                granny_file_reader * Reader);
GRANNY_DYNLINK(granny_file_reader *) GrannyCreatePlatformFileReader(char const * SourceFileName,
                                                                    granny_int32x SourceLineNumber,
                                                                    char const * FileNameToOpen);
GRANNY_DYNLINK(granny_open_file_reader_callback *) GrannyGetDefaultFileReaderOpenCallback(void);
GRANNY_DYNLINK(void) GrannySetDefaultFileReaderOpenCallback(granny_open_file_reader_callback * OpenFileReaderCallback);

/* granny_file_writer.h bindings: */
GRANNY_DYNLINK(granny_int32x) GrannyAlignWriter(granny_file_writer * Writer);
GRANNY_DYNLINK(granny_int32x) GrannyPredictWriterAlignment(granny_int32x Position);
GRANNY_DYNLINK(void) GrannyInitializeFileWriter(char const * SourceFileName,
                                                granny_int32x SourceLineNumber,
                                                granny_delete_file_writer_callback * DeleteFileWriterCallback,
                                                granny_seek_file_writer_callback * SeekWriterCallback,
                                                granny_write_file_writer_callback * WriteCallback,
                                                granny_begincrc_file_writer_callback * BeginCRCCallback,
                                                granny_endcrc_file_writer_callback * EndCRCCallback,
                                                granny_file_writer * Writer);
GRANNY_DYNLINK(granny_file_writer *) GrannyCreatePlatformFileWriter(char const * SourceFileName,
                                                                    granny_int32x SourceLineNumber,
                                                                    char const * FileNameToOpen,
                                                                    bool EraseExisting);
GRANNY_DYNLINK(granny_open_file_writer_callback *) GrannyGetDefaultFileWriterOpenCallback(void);
GRANNY_DYNLINK(void) GrannySetDefaultFileWriterOpenCallback(granny_open_file_writer_callback * OpenFileWriterCallback);
GRANNY_DYNLINK(granny_int32x) GrannySeekWriterFromStartStub(char const * SourceFileName,
                                                            granny_int32x SourceLineNumber,
                                                            granny_file_writer *  Writer,
                                                            granny_int32x OffsetInUInt8s);
GRANNY_DYNLINK(granny_int32x) GrannySeekWriterFromEndStub(char const * SourceFileName,
                                                          granny_int32x SourceLineNumber,
                                                          granny_file_writer *  Writer,
                                                          granny_int32x OffsetInUInt8s);
GRANNY_DYNLINK(granny_int32x) GrannySeekWriterFromCurrentPositionStub(char const * SourceFileName,
                                                                      granny_int32x SourceLineNumber,
                                                                      granny_file_writer *  Writer,
                                                                      granny_int32x OffsetInUInt8s);

/* granny_find_knot.h bindings: */
GRANNY_DYNLINK(granny_int32x) GrannyFindKnot(granny_int32x KnotCount,
                                             granny_real32 const * Knots,
                                             granny_real32 t);
GRANNY_DYNLINK(granny_int32x) GrannyFindCloseKnot(granny_int32x KnotCount,
                                                  granny_real32 const * Knots,
                                                  granny_real32 t,
                                                  granny_int32x StartingIndex);

/* granny_fixed_allocator.h bindings: */
GRANNY_DYNLINK(void *) GrannyAllocateFixed(granny_fixed_allocator * Allocator);
GRANNY_DYNLINK(void) GrannyDeallocateFixed(granny_fixed_allocator * Allocator,
                                           void * Memory);
GRANNY_DYNLINK(void) GrannyDeallocateAllFixed(granny_fixed_allocator * Allocator);
GRANNY_DYNLINK(void) GrannyInitializeFixedAllocator(granny_fixed_allocator * Allocator,
                                                    granny_int32x UnitSize);

/* granny_float16.h bindings: */
GRANNY_DYNLINK(granny_real16) GrannyReal32ToReal16(granny_real32 Value);
GRANNY_DYNLINK(void) GrannyReal16ToReal32(granny_real16 Value,
                                          granny_real32 *  Output);

/* granny_ik.h bindings: */
GRANNY_DYNLINK(void) GrannyIKUpdate(granny_int32x LinkCount,
                                    granny_int32x EEBoneIndex,
                                    granny_real32 const * DesiredPosition3,
                                    granny_int32x IterationCount,
                                    granny_skeleton const * Skeleton,
                                    granny_real32 const * ModelRoot4x4,
                                    granny_local_pose * LocalPose,
                                    granny_world_pose * WorldPose);
GRANNY_DYNLINK(bool) GrannyIKUpdate2Bone(granny_int32x EEBoneIndex,
                                         granny_real32 const * DesiredPosition3,
                                         granny_real32 const * RestrictedMovementPlaneNormal3,
                                         granny_skeleton const * Skeleton,
                                         granny_real32 const * ModelRootTransform,
                                         granny_local_pose * LocalPose,
                                         granny_world_pose * WorldPose,
                                         granny_real32 HyperExtensionStart,
                                         granny_real32 HyperExtensionScale);

/* granny_image_operations.h bindings: */
GRANNY_DYNLINK(void) GrannyScaleImage(granny_pixel_filter_type FilterType,
                                      granny_int32x SourceWidth,
                                      granny_int32x SourceHeight,
                                      granny_int32x SourceStride,
                                      granny_uint8 const * SourcePixels,
                                      granny_int32x DestWidth,
                                      granny_int32x DestHeight,
                                      granny_int32x DestStride,
                                      granny_uint8 * DestPixels);

/* granny_intersection.h bindings: */
GRANNY_DYNLINK(granny_int32x) GrannyRayIntersectsPlaneAt(granny_real32 const * PlaneNormal,
                                                         granny_real32 PlaneD,
                                                         granny_real32 const * RayOrigin,
                                                         granny_real32 const * RayNormal,
                                                         granny_real32 * T);
GRANNY_DYNLINK(bool) GrannyRayIntersectsSphere(granny_real32 const * Center,
                                               granny_real32 Radius,
                                               granny_real32 const * RayOrigin,
                                               granny_real32 const * RayNormal);
GRANNY_DYNLINK(granny_int32x) GrannyRayIntersectsSphereAt(granny_real32 const * Center,
                                                          granny_real32 Radius,
                                                          granny_real32 const * RayOrigin,
                                                          granny_real32 const * RayNormal,
                                                          granny_real32 * InT,
                                                          granny_real32 * OutT);
GRANNY_DYNLINK(granny_int32x) GrannyRayIntersectsBox(granny_real32 const * Transform4x4,
                                                     granny_real32 const * Min3,
                                                     granny_real32 const * Max3,
                                                     granny_real32 const * RayOrigin,
                                                     granny_real32 const * RayNormal);
GRANNY_DYNLINK(granny_int32x) GrannyRayIntersectsBoxAt(granny_real32 const * Transform4x4,
                                                       granny_real32 const * Min3,
                                                       granny_real32 const * Max3,
                                                       granny_real32 const * RayOrigin,
                                                       granny_real32 const * RayNormal,
                                                       granny_box_intersection * Intersection);
GRANNY_DYNLINK(granny_int32x) GrannyRayIntersectsTriangleAt(granny_real32 const * P0,
                                                            granny_real32 const * P1,
                                                            granny_real32 const * P2,
                                                            granny_real32 const * RayOrigin,
                                                            granny_real32 const * RayNormal,
                                                            granny_triangle_intersection * Intersection);

/* granny_local_pose.h bindings: */
GRANNY_DYNLINK(granny_local_pose *) GrannyNewLocalPose(granny_int32x BoneCount);
GRANNY_DYNLINK(void) GrannyFreeLocalPose(granny_local_pose * LocalPose);
GRANNY_DYNLINK(granny_int32x) GrannyGetResultingLocalPoseSize(granny_int32x BoneCount);
GRANNY_DYNLINK(granny_local_pose *) GrannyNewLocalPoseInPlace(granny_int32x BoneCount,
                                                              void * Memory);
GRANNY_DYNLINK(granny_int32x) GrannyGetLocalPoseBoneCount(granny_local_pose const * LocalPose);
GRANNY_DYNLINK(granny_transform *) GrannyGetLocalPoseTransform(granny_local_pose const * LocalPose,
                                                               granny_int32x BoneIndex);
GRANNY_DYNLINK(void * ) GrannyGetLocalPoseOpaqueTransformArray(granny_local_pose const*  LocalPose);
GRANNY_DYNLINK(void) GrannyBuildRestLocalPose(granny_skeleton const * Skeleton,
                                              granny_int32x FirstBone,
                                              granny_int32x BoneCount,
                                              granny_local_pose * LocalPose);
GRANNY_DYNLINK(void) GrannyBeginLocalPoseAccumulation(granny_local_pose * LocalPose,
                                                      granny_int32x FirstBone,
                                                      granny_int32x BoneCount,
                                                      granny_int32x const * SparseBoneArray);
GRANNY_DYNLINK(void) GrannyAccumulateLocalTransform(granny_local_pose * LocalPose,
                                                    granny_int32x LocalPoseBoneIndex,
                                                    granny_int32x SkeletonBoneIndex,
                                                    granny_real32 Weight,
                                                    granny_skeleton const * ReferenceSkeleton,
                                                    granny_quaternion_mode Mode,
                                                    granny_transform const * Transform);
GRANNY_DYNLINK(void) GrannyEndLocalPoseAccumulation(granny_local_pose * LocalPose,
                                                    granny_int32x FirstBone,
                                                    granny_int32x BoneCount,
                                                    granny_skeleton const * ReferenceSkeleton,
                                                    granny_int32x const * SparseBoneArray);
GRANNY_DYNLINK(void) GrannyEndLocalPoseAccumulationLOD(granny_local_pose * LocalPose,
                                                       granny_int32x FirstBone,
                                                       granny_int32x BoneCount,
                                                       granny_skeleton const * ReferenceSkeleton,
                                                       granny_real32 AllowedError,
                                                       granny_int32x const * SparseBoneArray);
GRANNY_DYNLINK(granny_real32) GrannyGetLocalPoseFillThreshold(granny_local_pose const * LocalPose);
GRANNY_DYNLINK(void) GrannySetLocalPoseFillThreshold(granny_local_pose * LocalPose,
                                                     granny_real32 FillThreshold);
GRANNY_DYNLINK(void) GrannyGetWorldMatrixFromLocalPose(granny_skeleton const * Skeleton,
                                                       granny_int32x BoneIndex,
                                                       granny_local_pose const * LocalPose,
                                                       granny_real32 const * Offset4x4,
                                                       granny_real32 * Result4x4,
                                                       granny_int32x const * SparseBoneArray,
                                                       granny_int32x const * SparseBoneArrayReverse);
GRANNY_DYNLINK(void) GrannyGetAttachmentOffset(granny_skeleton const * Skeleton,
                                               granny_int32x BoneIndex,
                                               granny_local_pose const * LocalPose,
                                               granny_real32 const * Offset4x4,
                                               granny_real32 * Result4x4,
                                               granny_int32x const * SparseBoneArray,
                                               granny_int32x const * SparseBoneArrayReverse);
GRANNY_DYNLINK(void) GrannyModulationCompositeLocalPoseSparse(granny_local_pose * BasePose,
                                                              granny_real32 WeightNone,
                                                              granny_real32 WeightAll,
                                                              granny_track_mask * Mask,
                                                              granny_local_pose const * CompositePose,
                                                              granny_int32x const * SparseBoneArray);
GRANNY_DYNLINK(void) GrannyLocalPoseFromWorldPose(granny_skeleton const*  Skeleton,
                                                  granny_local_pose * Result,
                                                  granny_world_pose const * WorldPose,
                                                  granny_real32 const * Offset4x4,
                                                  granny_int32x FirstBone,
                                                  granny_int32x BoneCount);
GRANNY_DYNLINK(void) GrannyLocalPoseFromWorldPoseNoScale(granny_skeleton const*  Skeleton,
                                                         granny_local_pose * Result,
                                                         granny_world_pose const * WorldPose,
                                                         granny_real32 const * Offset4x4,
                                                         granny_int32x FirstBone,
                                                         granny_int32x BoneCount);
GRANNY_DYNLINK(void) GrannyModulationCompositeLocalPose(granny_local_pose * BasePose,
                                                        granny_real32 WeightNone,
                                                        granny_real32 WeightAll,
                                                        granny_track_mask * Mask,
                                                        granny_local_pose const * CompositePose);
GRANNY_DYNLINK(void) GrannyCopyLocalPose(granny_local_pose const * Src,
                                         granny_local_pose * Dst);
GRANNY_DYNLINK(granny_int32x) GrannySparseBoneArrayCreateSingleBone(granny_skeleton const * Skeleton,
                                                                    granny_int32x FirstBoneRequired,
                                                                    granny_int32x * ResultSparseBoneArray,
                                                                    granny_int32x * ResultSparseBoneArrayReverse);
GRANNY_DYNLINK(granny_int32x) GrannySparseBoneArrayAddBone(granny_skeleton const * Skeleton,
                                                           granny_int32x BoneToAdd,
                                                           granny_int32x InitialBoneCount,
                                                           granny_int32x * SparseBoneArray,
                                                           granny_int32x * SparseBoneArrayReverse);
GRANNY_DYNLINK(void) GrannySparseBoneArrayExpand(granny_skeleton const * Skeleton,
                                                 granny_local_pose const * SourceSparseLocalPose,
                                                 granny_int32x SparseBoneCount,
                                                 granny_int32x const * SparseBoneArray,
                                                 granny_int32x const * SparseBoneArrayReverse,
                                                 granny_int32x BoneCount,
                                                 granny_local_pose * DestLocalPose);
GRANNY_DYNLINK(bool) GrannySparseBoneArrayIsValid(granny_int32x SkeletonBoneCount,
                                                  granny_int32x SparseBoneCount,
                                                  granny_int32x const * SparseBoneArray,
                                                  granny_int32x const * SparseBoneArrayReverse);
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyLocalPoseTransformType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyLocalPoseType;

/* granny_log.h bindings: */
GRANNY_DYNLINK(bool) GrannySetLogFileName(char const * FileName,
                                          bool Clear);
GRANNY_DYNLINK(void) GrannyGetLogCallback(granny_log_callback *  Result);
GRANNY_DYNLINK(void) GrannySetLogCallback(granny_log_callback const * LogCallback);
GRANNY_DYNLINK(char const* ) GrannyGetLogMessageTypeString(granny_log_message_type Type);
GRANNY_DYNLINK(char const* ) GrannyGetLogMessageOriginString(granny_log_message_origin Origin);
GRANNY_DYNLINK(bool) GrannyLogging(void);
GRANNY_DYNLINK(void) GrannyFilterMessage(granny_log_message_origin Origin,
                                         bool Enabled);
GRANNY_DYNLINK(void) GrannyFilterAllMessages(bool Enabled);
GRANNY_DYNLINK(granny_log_message_type) GrannyGetMostSeriousMessageType(void);
GRANNY_DYNLINK(char const *) GrannyGetMostSeriousMessage(void);
GRANNY_DYNLINK(void) GrannyClearMostSeriousMessage(void);

/* granny_math.h bindings: */
GRANNY_DYNLINK(void) GrannyEnsureQuaternionContinuity(granny_int32x QuaternionCount,
                                                      granny_real32 * Quaternions);
GRANNY_DYNLINK(bool) GrannyPolarDecompose(granny_real32 const * Source3x3,
                                          granny_real32 Tolerance,
                                          granny_real32 * Q3x3,
                                          granny_real32 * S3x3);
GRANNY_DYNLINK(void) GrannyColumnMatrixMultiply4x3(granny_real32 * IntoMatrix4x4,
                                                   granny_real32 const * Matrix4x4,
                                                   granny_real32 const * ByMatrix4x4);
GRANNY_DYNLINK(void) GrannyColumnMatrixMultiply4x3Transpose(granny_real32 * IntoMatrix4x3,
                                                            granny_real32 const * Matrix4x4,
                                                            granny_real32 const * ByMatrix4x4);
GRANNY_DYNLINK(void) GrannyColumnMatrixMultiply4x4(granny_real32 * IntoMatrix4x4,
                                                   granny_real32 const * Matrix4x4,
                                                   granny_real32 const * ByMatrix4x4);
GRANNY_DYNLINK(void) GrannyMatrixEqualsQuaternion3x3(granny_real32 * Dest,
                                                     granny_real32 const * Quaternion);
GRANNY_DYNLINK(void) GrannyQuaternionEqualsMatrix3x3(granny_real32 * Quaternion,
                                                     granny_real32 const * Matrix);
GRANNY_DYNLINK(void) GrannyInPlaceSimilarityTransformPosition(granny_real32 const * Affine3,
                                                              granny_real32 const * Linear3x3,
                                                              granny_real32 * Position3);
GRANNY_DYNLINK(void) GrannyInPlaceSimilarityTransformOrientation(granny_real32 const * Linear3x3,
                                                                 granny_real32 const * InverseLinear3x3,
                                                                 granny_real32 * Orientation4);
GRANNY_DYNLINK(void) GrannyInPlaceSimilarityTransformScaleShear(granny_real32 const * Linear3x3,
                                                                granny_real32 const * InverseLinear3x3,
                                                                granny_real32 * ScaleShear3x3);
GRANNY_DYNLINK(void) GrannyInPlaceSimilarityTransform(granny_real32 const * Affine3,
                                                      granny_real32 const * Linear3x3,
                                                      granny_real32 const * InverseLinear3x3,
                                                      granny_real32 * Position3,
                                                      granny_real32 * Orientation4,
                                                      granny_real32 * ScaleShear3x3);
GRANNY_DYNLINK(void) GrannyInPlaceSimilarityTransform4x3(granny_real32 const * Affine3,
                                                         granny_real32 const * Linear3x3,
                                                         granny_real32 const * InverseLinear3x3,
                                                         granny_real32 * Result4x4);

/* granny_memory.h bindings: */
GRANNY_DYNLINK(granny_allocation_header *) GrannyAllocationsBegin(void);
GRANNY_DYNLINK(granny_allocation_header *) GrannyNextAllocation(granny_allocation_header * Current);
GRANNY_DYNLINK(granny_allocation_header *) GrannyAllocationsEnd(void);
GRANNY_DYNLINK(void) GrannyGetAllocationInformation(granny_allocation_header const * Header,
                                                    granny_allocation_information * Information);
GRANNY_DYNLINK(void *) GrannyBeginAllocationCheck(void);
GRANNY_DYNLINK(granny_allocation_header *) GrannyCheckedAllocationsEnd(void * CheckIdentifier);
GRANNY_DYNLINK(bool) GrannyEndAllocationCheck(void * CheckIdentifier);
GRANNY_DYNLINK(void) GrannyGetAllocator(granny_allocate_callback ** AllocateCallback,
                                        granny_deallocate_callback ** DeallocateCallback);
GRANNY_DYNLINK(void) GrannySetAllocator(granny_allocate_callback * AllocateCallback,
                                        granny_deallocate_callback * DeallocateCallback);
GRANNY_DYNLINK(void) GrannyFreeBuilderResult(void * Result);
GRANNY_DYNLINK(void) GrannyAcquireMemorySpinlock(void);
GRANNY_DYNLINK(void) GrannyReleaseMemorySpinlock(void);

/* granny_memory_arena.h bindings: */
GRANNY_DYNLINK(granny_memory_arena *) GrannyNewMemoryArena(void);
GRANNY_DYNLINK(void) GrannySetArenaAlignment(granny_memory_arena *  Arena,
                                             granny_int32x Align);
GRANNY_DYNLINK(void) GrannyClearArena(granny_memory_arena * Arena);
GRANNY_DYNLINK(void) GrannyFreeMemoryArena(granny_memory_arena * Arena);
GRANNY_DYNLINK(void *) GrannyMemoryArenaPush(granny_memory_arena * Arena,
                                             granny_int32x Size);

/* granny_memory_file_reader.h bindings: */
GRANNY_DYNLINK(granny_file_reader *) GrannyCreateMemoryFileReader(char const * SourceFileName,
                                                                  granny_int32x SourceLineNumber,
                                                                  granny_int32x Offset,
                                                                  granny_int32x MemorySize,
                                                                  void * Memory);

/* granny_memory_file_writer.h bindings: */
GRANNY_DYNLINK(granny_file_writer *) GrannyCreateMemoryFileWriter(char const * SourceFileName,
                                                                  granny_int32x SourceLineNumber,
                                                                  granny_int32x BlockSize);
GRANNY_DYNLINK(bool) GrannyStealMemoryWriterBuffer(granny_file_writer * Writer,
                                                   granny_uint8 ** BufferPtr,
                                                   granny_int32x * BufferSize);
GRANNY_DYNLINK(void) GrannyFreeMemoryWriterBuffer(granny_uint8 *  Buffer);

/* granny_mesh.h bindings: */
GRANNY_DYNLINK(granny_int32x) GrannyGetMeshMorphTargetCount(granny_mesh const * Mesh);
GRANNY_DYNLINK(granny_int32x) GrannyGetMeshTriangleGroupCount(granny_mesh const * Mesh);
GRANNY_DYNLINK(granny_tri_material_group *) GrannyGetMeshTriangleGroups(granny_mesh const * Mesh);
GRANNY_DYNLINK(granny_data_type_definition *) GrannyGetMeshVertexType(granny_mesh const * Mesh);
GRANNY_DYNLINK(granny_data_type_definition *) GrannyGetMeshMorphVertexType(granny_mesh const * Mesh,
                                                                           granny_int32x MorphTargetIndex);
GRANNY_DYNLINK(granny_int32x) GrannyGetMeshVertexCount(granny_mesh const * Mesh);
GRANNY_DYNLINK(granny_int32x) GrannyGetMeshMorphVertexCount(granny_mesh const * Mesh,
                                                            granny_int32x MorphTargetIndex);
GRANNY_DYNLINK(void) GrannyCopyMeshVertices(granny_mesh const * Mesh,
                                            granny_data_type_definition const * VertexType,
                                            void * DestVertices);
GRANNY_DYNLINK(void) GrannyCopyMeshMorphVertices(granny_mesh const * Mesh,
                                                 granny_int32x MorphTargetIndex,
                                                 granny_data_type_definition const * VertexType,
                                                 void * DestVertices);
GRANNY_DYNLINK(void *) GrannyGetMeshVertices(granny_mesh const * Mesh);
GRANNY_DYNLINK(void *) GrannyGetMeshMorphVertices(granny_mesh const * Mesh,
                                                  granny_int32x MorphTargetIndex);
GRANNY_DYNLINK(bool) GrannyMeshIsRigid(granny_mesh const * Mesh);
GRANNY_DYNLINK(granny_int32x) GrannyGetMeshIndexCount(granny_mesh const * Mesh);
GRANNY_DYNLINK(granny_int32x) GrannyGetMeshTriangleCount(granny_mesh const * Mesh);
GRANNY_DYNLINK(granny_int32x) GrannyGetMeshBytesPerIndex(granny_mesh const * Mesh);
GRANNY_DYNLINK(void *) GrannyGetMeshIndices(granny_mesh const * Mesh);
GRANNY_DYNLINK(void) GrannyCopyMeshIndices(granny_mesh const * Mesh,
                                           granny_int32x BytesPerIndex,
                                           void * DestIndices);
GRANNY_DYNLINK(void) GrannyTransformBoundingBox(granny_real32 const * Affine3,
                                                granny_real32 const * Linear3x3,
                                                granny_real32 * OBBMin,
                                                granny_real32 * OBBMax);
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyBoneBindingType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyMaterialBindingType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyMorphTargetType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyMeshType;

/* granny_mesh_binding.h bindings: */
GRANNY_DYNLINK(granny_mesh_binding *) GrannyNewMeshBinding(granny_mesh const * Mesh,
                                                           granny_skeleton const * FromSkeleton,
                                                           granny_skeleton const * ToSkeleton);
GRANNY_DYNLINK(void) GrannyFreeMeshBinding(granny_mesh_binding * Binding);
GRANNY_DYNLINK(granny_int32x) GrannyGetResultingMeshBindingSize(granny_mesh const * Mesh,
                                                                granny_skeleton const * FromSkeleton,
                                                                granny_skeleton const * ToSkeleton);
GRANNY_DYNLINK(granny_mesh_binding *) GrannyNewMeshBindingInPlace(granny_mesh const * Mesh,
                                                                  granny_skeleton const * FromSkeleton,
                                                                  granny_skeleton const * ToSkeleton,
                                                                  void * Memory);
GRANNY_DYNLINK(bool) GrannyMeshBindingIsTransferred(granny_mesh_binding const * Binding);
GRANNY_DYNLINK(granny_int32x) GrannyGetMeshBinding4x4ArraySize(granny_mesh_binding const * Binding,
                                                               granny_int32x BoneCount);
GRANNY_DYNLINK(void) GrannyBuildMeshBinding4x4Array(granny_mesh_binding const * Binding,
                                                    granny_world_pose const * WorldPose,
                                                    granny_int32x FirstBoneIndex,
                                                    granny_int32x BoneCount,
                                                    granny_real32 * Matrix4x4Array);
GRANNY_DYNLINK(granny_int32x) GrannyGetMeshBindingBoneCount(granny_mesh_binding const * Binding);
GRANNY_DYNLINK(granny_int32x const *) GrannyGetMeshBindingFromBoneIndices(granny_mesh_binding const * Binding);
GRANNY_DYNLINK(granny_skeleton *) GrannyGetMeshBindingFromSkeleton(granny_mesh_binding const * Binding);
GRANNY_DYNLINK(granny_int32x const *) GrannyGetMeshBindingToBoneIndices(granny_mesh_binding const * Binding);
GRANNY_DYNLINK(granny_skeleton *) GrannyGetMeshBindingToSkeleton(granny_mesh_binding const * Binding);
GRANNY_DYNLINK(granny_mesh *) GrannyGetMeshBindingSourceMesh(granny_mesh_binding const * Binding);

/* granny_mesh_builder.h bindings: */
GRANNY_DYNLINK(granny_mesh_builder *) GrannyBeginMesh(granny_int32x VertexCount,
                                                      granny_int32x TriangleCount,
                                                      granny_int32x MaterialCount,
                                                      granny_int32x BoneCount,
                                                      granny_data_type_definition const * VertexType);
GRANNY_DYNLINK(void) GrannyGenerateTangentSpaceFromUVs(granny_mesh_builder * Builder);
GRANNY_DYNLINK(bool) GrannyEndMesh(granny_mesh_builder * Builder,
                                   granny_vertex_data ** VertexData,
                                   granny_tri_topology ** Topology);
GRANNY_DYNLINK(granny_int32x) GrannyGetResultingVertexCount(granny_mesh_builder const * Builder);
GRANNY_DYNLINK(void) GrannySerializeResultingCoincidentVertexMap(granny_mesh_builder const * Builder,
                                                                 granny_int32 * Dest);
GRANNY_DYNLINK(void) GrannySerializeResultingVertexToTriangleMap(granny_mesh_builder const * Builder,
                                                                 granny_int32 * Dest);
GRANNY_DYNLINK(void) GrannySerializeResultingVertices(granny_mesh_builder const * Builder,
                                                      void * Destination);
GRANNY_DYNLINK(granny_int32x) GrannyGetResultingVertexDataSize(granny_mesh_builder const * Builder);
GRANNY_DYNLINK(granny_int32x) GrannyGetResultingTopologySize(granny_mesh_builder const * Builder);
GRANNY_DYNLINK(void) GrannyEndMeshInPlace(granny_mesh_builder * Builder,
                                          void * VertexDataMemory,
                                          granny_vertex_data ** VertexData,
                                          void * TopologyMemory,
                                          granny_tri_topology ** Topology);
GRANNY_DYNLINK(void) GrannySetNormalTolerance(granny_mesh_builder * Builder,
                                              granny_real32 Tolerance);
GRANNY_DYNLINK(void) GrannySetTangentTolerance(granny_mesh_builder * Builder,
                                               granny_real32 Tolerance);
GRANNY_DYNLINK(void) GrannySetBinormalTolerance(granny_mesh_builder * Builder,
                                                granny_real32 Tolerance);
GRANNY_DYNLINK(void) GrannySetTangentBinormalCrossTolerance(granny_mesh_builder * Builder,
                                                            granny_real32 Tolerance);
GRANNY_DYNLINK(void) GrannySetTangentMergingTolerance(granny_mesh_builder * Builder,
                                                      granny_real32 MinCosine);
GRANNY_DYNLINK(void) GrannySetChannelTolerance(granny_mesh_builder * Builder,
                                               granny_int32x Channel,
                                               granny_real32 Tolerance);
GRANNY_DYNLINK(void) GrannySetVertexChannelComponentNames(granny_mesh_builder * Builder,
                                                          granny_int32x ChannelCount,
                                                          char const ** ChannelNames);
GRANNY_DYNLINK(void) GrannySetPosition(granny_mesh_builder * Builder,
                                       granny_real32 Px,
                                       granny_real32 Py,
                                       granny_real32 Pz);
GRANNY_DYNLINK(void) GrannyAddWeight(granny_mesh_builder * Builder,
                                     granny_int32x BoneIndex,
                                     granny_real32 Weight);
GRANNY_DYNLINK(void) GrannyPushVertex(granny_mesh_builder * Builder);
GRANNY_DYNLINK(void) GrannySetVertexIndex(granny_mesh_builder * Builder,
                                          granny_int32x Edge,
                                          granny_int32x Index);
GRANNY_DYNLINK(void) GrannySetNormal(granny_mesh_builder * Builder,
                                     granny_int32x Edge,
                                     granny_real32 Nx,
                                     granny_real32 Ny,
                                     granny_real32 Nz);
GRANNY_DYNLINK(void) GrannySetTangent(granny_mesh_builder * Builder,
                                      granny_int32x Edge,
                                      granny_real32 Tx,
                                      granny_real32 Ty,
                                      granny_real32 Tz);
GRANNY_DYNLINK(void) GrannySetBinormal(granny_mesh_builder * Builder,
                                       granny_int32x Edge,
                                       granny_real32 Bx,
                                       granny_real32 By,
                                       granny_real32 Bz);
GRANNY_DYNLINK(void) GrannySetTangentBinormalCross(granny_mesh_builder * Builder,
                                                   granny_int32x Edge,
                                                   granny_real32 TBCx,
                                                   granny_real32 TBCy,
                                                   granny_real32 TBCz);
GRANNY_DYNLINK(granny_int32x) GrannyGetChannelComponentCount(granny_mesh_builder * Builder,
                                                             granny_int32x ChannelIndex);
GRANNY_DYNLINK(void) GrannySetChannel(granny_mesh_builder * Builder,
                                      granny_int32x Edge,
                                      granny_int32x Channel,
                                      granny_real32 const * Value);
GRANNY_DYNLINK(void) GrannySetMaterial(granny_mesh_builder * Builder,
                                       granny_int32x MaterialIndex);
GRANNY_DYNLINK(void) GrannyPushTriangle(granny_mesh_builder * Builder);
GRANNY_DYNLINK(granny_int32x) GrannyGetOriginalVertex(granny_mesh_builder * Builder,
                                                      granny_int32x V);

/* granny_mesh_deformer.h bindings: */
GRANNY_DYNLINK(granny_mesh_deformer *) GrannyNewMeshDeformer(granny_data_type_definition const * InputVertexLayout,
                                                             granny_data_type_definition const * OutputVertexLayout,
                                                             granny_deformation_type DeformationType,
                                                             granny_deformer_tail_flags TailFlag);
GRANNY_DYNLINK(void) GrannyFreeMeshDeformer(granny_mesh_deformer * Deformer);
GRANNY_DYNLINK(void) GrannyDeformVertices(granny_mesh_deformer const * Deformer,
                                          granny_int32x const * MatrixIndices,
                                          granny_real32 const * MatrixBuffer4x4,
                                          granny_int32x VertexCount,
                                          void const * SourceVertices,
                                          void * DestVertices);

/* granny_oodle1_compression.h bindings: */
GRANNY_DYNLINK(granny_oodle1_state *) GrannyOodle1BeginSimple(granny_uint32x ExpandedDataSize,
                                                              granny_int32x BufferCount);
GRANNY_DYNLINK(granny_int32x) GrannyGetOodle1CompressBufferPaddingSize(void);
GRANNY_DYNLINK(void) GrannyOodle1Compress(granny_oodle1_state * Oodle1State,
                                          granny_int32x BufferSize,
                                          void const * Buffer);
GRANNY_DYNLINK(granny_int32x) GrannyOodle1End(granny_oodle1_state * Oodle1State,
                                              void ** Buffer,
                                              bool WritingForReversedPlatform);
GRANNY_DYNLINK(void) GrannyOodle1FreeSimple(granny_oodle1_state * Oodle1State);
GRANNY_DYNLINK(granny_int32x) GrannyGetOodle1DecompressBufferPaddingSize(void);
GRANNY_DYNLINK(bool) GrannyOodle1Decompress(bool FileIsByteReversed,
                                            granny_int32x CompressedBytesSize,
                                            void const * CompressedBytes,
                                            granny_int32x DecompressedSize,
                                            void * DecompressedBytes);

/* granny_periodic_loop.h bindings: */
GRANNY_DYNLINK(void) GrannyZeroPeriodicLoop(granny_periodic_loop * Loop);
GRANNY_DYNLINK(void) GrannyFitPeriodicLoop(granny_real32 const * StartPosition3,
                                           granny_real32 const * StartOrientation4,
                                           granny_real32 const * EndPosition3,
                                           granny_real32 const * EndOrientation4,
                                           granny_real32 Seconds,
                                           granny_periodic_loop * Loop);
GRANNY_DYNLINK(void) GrannyComputePeriodicLoopVector(granny_periodic_loop const * Loop,
                                                     granny_real32 Seconds,
                                                     granny_real32 * Result3);
GRANNY_DYNLINK(void) GrannyComputePeriodicLoopLog(granny_periodic_loop const * Loop,
                                                  granny_real32 Seconds,
                                                  granny_real32 * Result4);
GRANNY_DYNLINK(void) GrannyStepPeriodicLoop(granny_periodic_loop const * Loop,
                                            granny_real32 Seconds,
                                            granny_real32 * Position3,
                                            granny_real32 * Orientation4);
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPeriodicLoopType;

/* granny_pointer_hash.h bindings: */
GRANNY_DYNLINK(granny_pointer_hash *) GrannyNewPointerHash(void);
GRANNY_DYNLINK(void) GrannyDeletePointerHash(granny_pointer_hash * Hash);
GRANNY_DYNLINK(void) GrannyAddPointerToHash(granny_pointer_hash * Hash,
                                            void const * Key,
                                            void * Data);
GRANNY_DYNLINK(bool) GrannySetHashedPointerData(granny_pointer_hash * Hash,
                                                void const * Key,
                                                void * Data);
GRANNY_DYNLINK(bool) GrannyGetHashedPointerData(granny_pointer_hash * Hash,
                                                void const * Key,
                                                void ** Data);
GRANNY_DYNLINK(bool) GrannyHashedPointerKeyExists(granny_pointer_hash * Hash,
                                                  void const * Key);

/* granny_s3tc.h bindings: */
GRANNY_DYNLINK(granny_pixel_layout const *) GrannyGetS3TCPixelLayout(granny_s3tc_texture_format Format);
GRANNY_DYNLINK(char const *) GrannyGetS3TCTextureFormatName(granny_int32x Format);

/* granny_skeleton.h bindings: */
GRANNY_DYNLINK(void) GrannyBuildSkeletonRelativeTransform(granny_int32x SourceTransformStride,
                                                          granny_transform const * SourceTransforms,
                                                          granny_int32x SourceParentStride,
                                                          granny_int32 const * SourceParents,
                                                          granny_int32x BoneIndex,
                                                          granny_transform * Result);
GRANNY_DYNLINK(void) GrannyBuildSkeletonRelativeTransforms(granny_int32x SourceTransformStride,
                                                           granny_transform const * SourceTransforms,
                                                           granny_int32x SourceParentStride,
                                                           granny_int32 const * SourceParents,
                                                           granny_int32x Count,
                                                           granny_int32x ResultStride,
                                                           granny_transform * Results);
GRANNY_DYNLINK(bool) GrannyFindBoneByName(granny_skeleton const * Skeleton,
                                          char const * BoneName,
                                          granny_int32x * BoneIndex);
GRANNY_DYNLINK(bool) GrannyFindBoneByNameLowercase(granny_skeleton const * Skeleton,
                                                   char const * BoneName,
                                                   granny_int32x * BoneIndex);
GRANNY_DYNLINK(bool) GrannyFindBoneByRule(granny_skeleton const*  Skeleton,
                                          char const * ProcessedBoneName,
                                          char const * BonePattern,
                                          granny_int32x * BoneIndex);
GRANNY_DYNLINK(int) GrannyGetBoneCountForLOD(granny_skeleton const*  Skeleton,
                                             granny_real32 AllowedError);
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyBoneType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannySkeletonType;

/* granny_skeleton_builder.h bindings: */
GRANNY_DYNLINK(granny_skeleton_builder *) GrannyBeginSkeleton(granny_int32x BoneCount);
GRANNY_DYNLINK(granny_skeleton *) GrannyEndSkeleton(granny_skeleton_builder * Builder,
                                                    granny_int32x * RemappingTable);
GRANNY_DYNLINK(granny_int32x) GrannyGetResultingSkeletonSize(granny_skeleton_builder const * Builder);
GRANNY_DYNLINK(granny_skeleton *) GrannyEndSkeletonInPlace(granny_skeleton_builder * Builder,
                                                           void * InMemory,
                                                           granny_int32x * RemappingTable);
GRANNY_DYNLINK(void) GrannyAddBone(granny_skeleton_builder * Builder,
                                   granny_real32 const * LocalPosition3,
                                   granny_real32 const * LocalOrientation4,
                                   granny_real32 const * LocalScaleShear3x3,
                                   granny_real32 const * WorldPosition3,
                                   granny_real32 const * WorldOrientation4,
                                   granny_real32 const * WorldScaleShear3x3);
GRANNY_DYNLINK(void) GrannyAddBoneWithInverse(granny_skeleton_builder * Builder,
                                              granny_real32 const * LocalPosition3,
                                              granny_real32 const * LocalOrientation4,
                                              granny_real32 const * LocalScaleShear3x3,
                                              granny_matrix_4x4 const*  InverseWorld4x4);
GRANNY_DYNLINK(void) GrannySetBoneParent(granny_skeleton_builder * Builder,
                                         granny_int32x BoneIndex,
                                         granny_int32x ParentIndex);
GRANNY_DYNLINK(void) GrannySetBoneLODError(granny_skeleton_builder * Builder,
                                           granny_int32x BoneIndex,
                                           granny_real32 LODError);

/* granny_spu_animation.h bindings: */
GRANNY_DYNLINK(bool) GrannyFindSPUTrackGroupForModel(granny_spu_animation const*  Animation,
                                                     char const * ModelName,
                                                     granny_int32x * TrackGroupIndex);
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannySPUAnimationType;

/* granny_spu_animation_binding.h bindings: */
GRANNY_DYNLINK(void) GrannyMakeDefaultSPUAnimationBindingID(granny_spu_animation_binding_id * ID,
                                                            granny_spu_animation const*  Animation,
                                                            granny_int32x TrackGroupIndex,
                                                            granny_model const*  Model);
GRANNY_DYNLINK(granny_spu_animation_binding *) GrannyAcquireSPUAnimationBindingFromID(granny_spu_animation_binding_id * ID);
GRANNY_DYNLINK(granny_spu_animation_binding *) GrannyAcquireSPUAnimationBinding(granny_spu_animation_binding * Binding);
GRANNY_DYNLINK(void) GrannyReleaseSPUAnimationBinding(granny_spu_animation_binding * Binding);
GRANNY_DYNLINK(granny_int32x) GrannyGetMaximumSPUAnimationBindingCount(void);
GRANNY_DYNLINK(void) GrannySetMaximumSPUAnimationBindingCount(granny_int32x BindingCountMax);

/* granny_spu_animation_info.h bindings: */
GRANNY_DYNLINK(granny_spu_animation_info * ) GrannyGetSPUAnimationInfo(granny_file * File);
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannySPUAnimationInfoType;

/* granny_spu_controlled_animation.h bindings: */
GRANNY_DYNLINK(granny_control *) GrannyPlayControlledSPUAnimation(granny_real32 StartTime,
                                                                  granny_spu_animation const * Animation,
                                                                  granny_model_instance * Model);

/* granny_spu_track_group.h bindings: */
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannySPUTransformTrackType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannySPUTrackGroupType;

/* granny_stack_allocator.h bindings: */
GRANNY_DYNLINK(void) GrannyStackInitialize(granny_stack_allocator * Allocator,
                                           granny_int32x UnitSize,
                                           granny_int32x UnitsPerBlock);
GRANNY_DYNLINK(void) GrannyStackInitializeWithDirectory(granny_stack_allocator * Allocator,
                                                        granny_int32x UnitSize,
                                                        granny_int32x UnitsPerBlock,
                                                        granny_int32x MaxUnits);
GRANNY_DYNLINK(void) GrannyStackCleanUp(granny_stack_allocator * Allocator);
GRANNY_DYNLINK(granny_int32x) GrannyGetStackUnitCount(granny_stack_allocator const * Allocator);
GRANNY_DYNLINK(void * ) GrannyNewStackUnit(granny_stack_allocator * Allocator,
                                           granny_int32x * Result);
GRANNY_DYNLINK(void *) GrannyGetStackUnit(granny_stack_allocator * Allocator,
                                          granny_int32x Index);
GRANNY_DYNLINK(void) GrannyPopStackUnits(granny_stack_allocator * Allocator,
                                         granny_int32x UnitCount);
GRANNY_DYNLINK(bool) GrannyMultipleNewStackUnits(granny_stack_allocator * Allocator,
                                                 granny_int32x NumNewIndices,
                                                 granny_int32x * NewIndicesStart,
                                                 void const*  InitialValue);
GRANNY_DYNLINK(void) GrannySerializeStack(granny_stack_allocator const * Allocator,
                                          void * Dest);

/* granny_statistics.h bindings: */
GRANNY_DYNLINK(granny_int32x) GrannyGetCounterCount(void);
GRANNY_DYNLINK(granny_real64x) GrannyGetCounterTicksPerSecond(void);
GRANNY_DYNLINK(void) GrannyResetCounters(void);
GRANNY_DYNLINK(void) GrannyResetCounterPeaks(void);
GRANNY_DYNLINK(void) GrannyGetCounterResults(granny_int32x CounterIndex,
                                             granny_counter_results * Results);

/* granny_stat_hud.h bindings: */
GRANNY_DYNLINK(granny_stat_hud *) GrannyCaptureCurrentStats(granny_int32x FramesSinceLastCapture);
GRANNY_DYNLINK(void) GrannyFreeStats(granny_stat_hud * StatHUD);
GRANNY_DYNLINK(char **) GrannyDumpStatHUD(granny_stat_hud * StatHUD);
GRANNY_DYNLINK(void) GrannyFreeStatHUDDump(char ** StatHUDDump);

/* granny_string.h bindings: */
GRANNY_DYNLINK(granny_int32x) GrannyStringDifference(char const * StringA,
                                                     char const * StringB);
GRANNY_DYNLINK(void) GrannySetStringComparisonCallback(granny_string_comparison_callback * Callback);

/* granny_string_database.h bindings: */
GRANNY_DYNLINK(granny_string_database *) GrannyGetStringDatabase(granny_file * File);
GRANNY_DYNLINK(bool) GrannyRemapFileStrings(granny_file *  File,
                                            granny_string_database *  StringDatabase);
GRANNY_DYNLINK(char *) GrannyRebaseToStringDatabase(void * Data,
                                                    granny_uint32 Identifier);
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyStringDatabaseType;

/* granny_string_table.h bindings: */
GRANNY_DYNLINK(granny_string_table *) GrannyNewStringTable(void);
GRANNY_DYNLINK(void) GrannyFreeStringTable(granny_string_table * Table);
GRANNY_DYNLINK(char const *) GrannyMapString(granny_string_table * Table,
                                             char const * String);

/* granny_system_clock.h bindings: */
GRANNY_DYNLINK(void) GrannyGetSystemSeconds(granny_system_clock *  Result);
GRANNY_DYNLINK(granny_real32) GrannyGetSecondsElapsed(granny_system_clock const * StartClock,
                                                      granny_system_clock const * EndClock);
GRANNY_DYNLINK(void) GrannySleepForSeconds(granny_real32 Seconds);

/* granny_tangent_frame.h bindings: */
GRANNY_DYNLINK(bool) GrannyBuildTangentSpace(granny_mesh_builder *  Builder);
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyGBX333VertexType;

/* granny_texture.h bindings: */
GRANNY_DYNLINK(char const *) GrannyGetTextureTypeName(granny_int32x TextureType);
GRANNY_DYNLINK(char const *) GrannyGetTextureEncodingName(granny_int32x Encoding);
GRANNY_DYNLINK(granny_int32x) GrannyGetRawImageSize(granny_pixel_layout const * Layout,
                                                    granny_int32x Stride,
                                                    granny_int32x Width,
                                                    granny_int32x Height);
GRANNY_DYNLINK(granny_int32x) GrannyGetS3TCImageSize(granny_s3tc_texture_format Format,
                                                     granny_int32x Width,
                                                     granny_int32x Height);
GRANNY_DYNLINK(bool) GrannyGetRecommendedPixelLayout(granny_texture const * Texture,
                                                     granny_pixel_layout * Layout);
GRANNY_DYNLINK(bool) GrannyCopyTextureImage(granny_texture const * Texture,
                                            granny_int32x ImageIndex,
                                            granny_int32x MIPIndex,
                                            granny_pixel_layout const * Layout,
                                            granny_int32x DestWidth,
                                            granny_int32x DestHeight,
                                            granny_int32x DestStride,
                                            void * Pixels);
GRANNY_DYNLINK(bool) GrannyTextureHasAlpha(granny_texture const * Texture);
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyTextureMIPLevelType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyTextureImageType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyTextureType;

/* granny_texture_builder.h bindings: */
GRANNY_DYNLINK(granny_texture_builder *) GrannyBeginRawTexture(granny_int32x Width,
                                                               granny_int32x Height,
                                                               granny_pixel_layout const * ResultingLayout,
                                                               granny_int32x StrideAlignment);
GRANNY_DYNLINK(granny_texture_builder *) GrannyBeginS3TCTexture(granny_int32x Width,
                                                                granny_int32x Height,
                                                                granny_s3tc_texture_format Format);
GRANNY_DYNLINK(granny_texture_builder *) GrannyBeginBestMatchS3TCTexture(granny_int32x Width,
                                                                         granny_int32x Height);
GRANNY_DYNLINK(granny_texture_builder *) GrannyBeginBinkTexture(granny_int32x Width,
                                                                granny_int32x Height,
                                                                granny_int32x Compression,
                                                                granny_uint32x Flags);
GRANNY_DYNLINK(granny_texture *) GrannyEndTexture(granny_texture_builder * Builder);
GRANNY_DYNLINK(granny_int32x) GrannyGetResultingTextureSize(granny_texture_builder const * Builder);
GRANNY_DYNLINK(granny_texture *) GrannyEndTextureInPlace(granny_texture_builder * Builder,
                                                         void * Memory);
GRANNY_DYNLINK(void) GrannySetImageScalingFilter(granny_texture_builder * Builder,
                                                 granny_pixel_filter_type UpsamplingFilter,
                                                 granny_pixel_filter_type DownsamplingFilter);
GRANNY_DYNLINK(void) GrannyEncodeImage(granny_texture_builder * Builder,
                                       granny_int32x Width,
                                       granny_int32x Height,
                                       granny_int32x Stride,
                                       granny_int32x MIPLevelCount,
                                       void const * RGBAData);

/* granny_thread_safety.h bindings: */
GRANNY_DYNLINK(void) GrannySetAllowGlobalStateChanges(bool Allowed);
GRANNY_DYNLINK(bool) GrannyGetAllowGlobalStateChanges(void);
GRANNY_DYNLINK(granny_thread_id_callback *) GrannySetThreadIDCallback(granny_thread_id_callback * Callback);
GRANNY_DYNLINK(bool) GrannyThreadAllowedToCallGranny(void);

/* granny_track_group.h bindings: */
GRANNY_DYNLINK(void) GrannyGetTrackGroupInitialPlacement4x4(granny_track_group * TrackGroup,
                                                            granny_real32 * Placement4x4);
GRANNY_DYNLINK(void) GrannyTransformCurve3(granny_transform const * Transform,
                                           granny_int32x Count,
                                           granny_real32 * Curve3);
GRANNY_DYNLINK(void) GrannyTransformCurve4(granny_transform const * Transform,
                                           granny_int32x Count,
                                           granny_real32 * Curve4);
GRANNY_DYNLINK(void) GrannyTransformCurve3x3(granny_transform const * Transform,
                                             granny_int32x Count,
                                             granny_real32 * Curve3x3);
GRANNY_DYNLINK(void) GrannyGetTrackInitialTransform(granny_transform_track const * Track,
                                                    granny_transform * Transform);
GRANNY_DYNLINK(void) GrannyRemoveTrackInitialTransform(granny_transform_track * Track);
GRANNY_DYNLINK(bool) GrannyBasisConversionRequiresCurveDecompression(granny_real32 * Affine3,
                                                                     granny_real32 * Linear3x3,
                                                                     granny_real32 * InverseLinear3x3,
                                                                     granny_real32 AffineTolerance,
                                                                     granny_real32 LinearTolerance,
                                                                     bool RoundToTolerance);
GRANNY_DYNLINK(void) GrannyTransformCurveVec3(granny_real32 const * Affine3,
                                              granny_real32 const * Linear3x3,
                                              granny_real32 AffineTolerance,
                                              granny_real32 LinearTolerance,
                                              granny_curve2 * Curve);
GRANNY_DYNLINK(void) GrannySimilarityTransformCurvePosition(granny_real32 const * Affine3,
                                                            granny_real32 const * Linear3x3,
                                                            granny_real32 const * InverseLinear3x3,
                                                            granny_real32 AffineTolerance,
                                                            granny_real32 LinearTolerance,
                                                            granny_curve2 * Curve);
GRANNY_DYNLINK(void) GrannySimilarityTransformCurveQuaternion(granny_real32 const * Affine3,
                                                              granny_real32 const * Linear3x3,
                                                              granny_real32 const * InverseLinear3x3,
                                                              granny_real32 AffineTolerance,
                                                              granny_real32 LinearTolerance,
                                                              granny_curve2 * Curve);
GRANNY_DYNLINK(void) GrannySimilarityTransformCurveScaleShear(granny_real32 const * Affine3,
                                                              granny_real32 const * Linear3x3,
                                                              granny_real32 const * InverseLinear3x3,
                                                              granny_real32 AffineTolerance,
                                                              granny_real32 LinearTolerance,
                                                              granny_curve2 * Curve);
GRANNY_DYNLINK(void) GrannySimilarityTransformTrackGroup(granny_track_group * TrackGroup,
                                                         granny_real32 const * Affine3,
                                                         granny_real32 const * Linear3x3,
                                                         granny_real32 const * InverseLinear3x3,
                                                         granny_real32 AffineTolerance,
                                                         granny_real32 LinearTolerance);
GRANNY_DYNLINK(void) GrannyGetVectorDifferences(granny_int32x VectorDimension,
                                                granny_int32x VectorCount,
                                                granny_real32 const * Vectors,
                                                granny_real32 const * Identity,
                                                granny_vector_diff_mode DiffMode,
                                                granny_real32 * IdentityDifference,
                                                granny_real32 * ConstantDifference);
GRANNY_DYNLINK(bool) GrannyKnotsAreReducible(granny_int32x Degree,
                                             granny_int32x Dimension,
                                             granny_int32x KnotCount,
                                             granny_real32 const * Knots,
                                             granny_real32 const * Controls,
                                             granny_real32 const * Identity,
                                             granny_real32 Epsilon,
                                             granny_vector_diff_mode DiffMode,
                                             granny_int32x * RequiredDegree,
                                             granny_int32x * RequiredKnotCount);
GRANNY_DYNLINK(bool) GrannyCurveIsReducible(granny_curve2 const * Curve,
                                            granny_real32 const * Identity,
                                            granny_real32 Epsilon,
                                            granny_vector_diff_mode DiffMode,
                                            granny_int32x * RequiredDegree,
                                            granny_int32x * RequiredKnotCount);
GRANNY_DYNLINK(bool) GrannyTransformTrackHasKeyframedCurves(granny_transform_track const * Track);
GRANNY_DYNLINK(bool) GrannyTransformTrackIsAnimated(granny_transform_track const * Track);
GRANNY_DYNLINK(bool) GrannyTransformTrackIsIdentity(granny_transform_track const * Track);
GRANNY_DYNLINK(bool) GrannyFindTrackByName(granny_track_group const * TrackGroup,
                                           char const * TrackName,
                                           granny_int32x * TrackIndex);
GRANNY_DYNLINK(bool) GrannyFindTrackByRule(granny_track_group const * TrackGroup,
                                           char const * ProcessedTrackName,
                                           char const * TrackPattern,
                                           granny_int32x * TrackIndex);
GRANNY_DYNLINK(bool) GrannyFindVectorTrackByName(granny_track_group const * TrackGroup,
                                                 char const * TrackName,
                                                 granny_int32x * VectorTrackIndex);
GRANNY_DYNLINK(bool) GrannyFindVectorTrackByRule(granny_track_group const * TrackGroup,
                                                 char const * ProcessedTrackName,
                                                 char const * TrackPattern,
                                                 granny_int32x * VectorTrackIndex);
GRANNY_DYNLINK(void) GrannyGetTrackGroupFlags(granny_track_group const * TrackGroup,
                                              granny_uint32 * Flags,
                                              granny_uint32 * VDADOFs);
GRANNY_DYNLINK(void) GrannySetTrackGroupFlags(granny_track_group * TrackGroup,
                                              granny_uint32 Flags,
                                              granny_uint32 VDADOFs);
GRANNY_DYNLINK(granny_uint32) GrannyVectorTrackKeyForBone(granny_skeleton *    Skeleton,
                                                          granny_int32x BoneIndex,
                                                          char const*  TrackName);
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyVectorTrackType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyTransformTrackType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyTextTrackEntryType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyTextTrackType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyTrackGroupType;

/* granny_track_group_builder.h bindings: */
GRANNY_DYNLINK(granny_track_group_builder *) GrannyBeginTrackGroup(char const * Name,
                                                                   granny_int32x VectorTrackCount,
                                                                   granny_int32x TransformTrackCount,
                                                                   granny_int32x TextTrackCount,
                                                                   bool IncludeLODErrorSpace);
GRANNY_DYNLINK(granny_track_group *) GrannyEndTrackGroup(granny_track_group_builder * Builder);
GRANNY_DYNLINK(granny_int32x) GrannyGetResultingTrackGroupSize(granny_track_group_builder const * Builder);
GRANNY_DYNLINK(granny_track_group *) GrannyEndTrackGroupInPlace(granny_track_group_builder * Builder,
                                                                void * Memory);
GRANNY_DYNLINK(void) GrannyPushVectorTrackCurve(granny_track_group_builder * Builder,
                                                char const * Name,
                                                granny_uint32 TrackKey,
                                                granny_curve2 const * SourceCurve);
GRANNY_DYNLINK(void) GrannyBeginTransformTrack(granny_track_group_builder * Builder,
                                               char const * Name,
                                               granny_int32x Flags);
GRANNY_DYNLINK(void) GrannySetTransformTrackPositionCurve(granny_track_group_builder * Builder,
                                                          granny_curve2 const * SourceCurve);
GRANNY_DYNLINK(void) GrannySetTransformTrackOrientationCurve(granny_track_group_builder * Builder,
                                                             granny_curve2 const * SourceCurve);
GRANNY_DYNLINK(void) GrannySetTransformTrackScaleShearCurve(granny_track_group_builder * Builder,
                                                            granny_curve2 const * SourceCurve);
GRANNY_DYNLINK(void) GrannyEndTransformTrack(granny_track_group_builder * Builder);
GRANNY_DYNLINK(void) GrannyBeginTextTrack(granny_track_group_builder * Builder,
                                          char const * Name);
GRANNY_DYNLINK(void) GrannyAddTextEntry(granny_track_group_builder * Builder,
                                        granny_real32 TimeStamp,
                                        char const * Text);
GRANNY_DYNLINK(void) GrannyEndTextTrack(granny_track_group_builder * Builder);
GRANNY_DYNLINK(void) GrannyResortTrackGroup(granny_track_group * Group);
GRANNY_DYNLINK(void) GrannyAllocateLODErrorSpace(granny_track_group * Group);
GRANNY_DYNLINK(void) GrannyFreeLODErrorSpace(granny_track_group * Group);
GRANNY_DYNLINK(void) GrannySetAllLODErrorSpace(granny_track_group * Group,
                                               granny_real32 Value);
GRANNY_DYNLINK(void) GrannyResetLODErrorSpace(granny_track_group * Group);

/* granny_track_group_sampler.h bindings: */
GRANNY_DYNLINK(granny_track_group_sampler *) GrannyBeginSampledAnimation(granny_int32x TransformCurveCount,
                                                                         granny_int32x SampleCount);
GRANNY_DYNLINK(void) GrannyEndSampledAnimation(granny_track_group_sampler * Sampler);
GRANNY_DYNLINK(granny_track_group_sampler *) GrannyBeginSampledAnimationNonBlocked(granny_int32x TransformCurveCount,
                                                                                   granny_int32x SampleCount);
GRANNY_DYNLINK(granny_real32 *) GrannyGetPositionSamples(granny_track_group_sampler * Sampler,
                                                         granny_int32x TrackIndex);
GRANNY_DYNLINK(granny_real32 *) GrannyGetOrientationSamples(granny_track_group_sampler * Sampler,
                                                            granny_int32x TrackIndex);
GRANNY_DYNLINK(granny_real32 *) GrannyGetScaleShearSamples(granny_track_group_sampler * Sampler,
                                                           granny_int32x TrackIndex);
GRANNY_DYNLINK(void) GrannySetTransformSample(granny_track_group_sampler * Sampler,
                                              granny_int32x TrackIndex,
                                              granny_real32 const * Position3,
                                              granny_real32 const * Orientation4,
                                              granny_real32 const * ScaleShear3x3);
GRANNY_DYNLINK(void) GrannyPushSampledFrame(granny_track_group_sampler * Sampler);

/* granny_track_mask.h bindings: */
GRANNY_DYNLINK(granny_track_mask *) GrannyNewTrackMask(granny_real32 DefaultWeight,
                                                       granny_int32x BoneCount);
GRANNY_DYNLINK(granny_extract_track_mask_result) GrannyExtractTrackMask(granny_track_mask * TrackMask,
                                                                        granny_int32x BoneCount,
                                                                        granny_skeleton const * Skeleton,
                                                                        char * MaskName,
                                                                        granny_real32 DefaultWeight,
                                                                        bool UseParentForDefault);
GRANNY_DYNLINK(granny_real32) GrannyGetTrackMaskBoneWeight(granny_track_mask const * Mask,
                                                           granny_int32x BoneIndex);
GRANNY_DYNLINK(void) GrannySetTrackMaskBoneWeight(granny_track_mask * Mask,
                                                  granny_int32x BoneIndex,
                                                  granny_real32 Weight);
GRANNY_DYNLINK(void) GrannyFreeTrackMask(granny_track_mask * Mask);
GRANNY_DYNLINK(granny_track_mask *) GrannyCopyTrackMask(granny_track_mask const * Mask);
GRANNY_DYNLINK(void) GrannyInvertTrackMask(granny_track_mask * Mask);
GRANNY_DYNLINK(void) GrannySetSkeletonTrackMaskFromTrackGroup(granny_track_mask * Mask,
                                                              granny_skeleton const * Skeleton,
                                                              granny_track_group const * TrackGroup,
                                                              granny_real32 IdentityValue,
                                                              granny_real32 ConstantValue,
                                                              granny_real32 AnimatedValue);
GRANNY_DYNLINK(void) GrannySetSkeletonTrackMaskChainUpwards(granny_track_mask * Mask,
                                                            granny_skeleton const * Skeleton,
                                                            granny_int32x ChainLeafBoneIndex,
                                                            granny_real32 Weight);
GRANNY_DYNLINK(void) GrannySetSkeletonTrackMaskChainDownwards(granny_track_mask * Mask,
                                                              granny_skeleton const * Skeleton,
                                                              granny_int32x ChainRootBoneIndex,
                                                              granny_real32 Weight);
GRANNY_DYNLINK(granny_int32x) GrannyFindMaskIndexForName(granny_unbound_track_mask *  UnboundMask,
                                                         char const*  Name);
GRANNY_DYNLINK(void) GrannyBindTrackmaskToModel(granny_unbound_track_mask *  UnboundMask,
                                                granny_model *  Model,
                                                granny_track_mask *  Mask);
GRANNY_DYNLINK(void) GrannyBindTrackmaskToTrackGroup(granny_unbound_track_mask *  UnboundMask,
                                                     granny_track_group *  Model,
                                                     granny_track_mask *  Mask);
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyTrackMaskType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyUnboundTrackMaskType;
extern GRANNY_DYNLINKDATA(granny_track_mask *) GrannyIdentityTrackMask;
extern GRANNY_DYNLINKDATA(granny_track_mask *) GrannyNullTrackMask;

/* granny_track_sampler.h bindings: */
GRANNY_DYNLINK(void) GrannySampleTrackUUULocal(granny_sample_context const * Context,
                                               granny_transform_track const * SourceTrack,
                                               granny_bound_transform_track * Track,
                                               granny_transform * Result);
GRANNY_DYNLINK(void) GrannySampleTrackPOLocal(granny_sample_context const * Context,
                                              granny_transform_track const * SourceTrack,
                                              granny_bound_transform_track * Track,
                                              granny_real32 * ResultPosition,
                                              granny_real32 * ResultOrientation);
GRANNY_DYNLINK(void) GrannySampleTrackUUULocalAtTime0(granny_transform_track const * SourceTrack,
                                                      granny_transform * Result);
GRANNY_DYNLINK(granny_track_sampler *) GrannyGetTrackSamplerFor(granny_transform_track const * Track);
GRANNY_DYNLINK(granny_track_sampler *) GrannyGetTrackSamplerUUU(void);
GRANNY_DYNLINK(granny_track_sampler *) GrannyGetTrackSamplerSSS(void);
GRANNY_DYNLINK(granny_track_sampler *) GrannyGetTrackSamplerIII(void);
GRANNY_DYNLINK(granny_track_sampler *) GrannyGetTrackSamplerIIU(void);

/* granny_tri_topology.h bindings: */
GRANNY_DYNLINK(void) GrannyInvertTriTopologyWinding(granny_tri_topology * Topology);
GRANNY_DYNLINK(void) GrannyRemapTopologyMaterials(granny_tri_topology * Topology,
                                                  granny_int32x RemapCount,
                                                  granny_int32x * RemapTable);
GRANNY_DYNLINK(void) GrannyConvertIndices(granny_int32x IndexCount,
                                          granny_int32x FromBytesPerIndex,
                                          void const * FromIndices,
                                          granny_int32x ToBytesPerIndex,
                                          void * ToIndices);
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyTriMaterialGroupType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyTriAnnotationSetType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyTriTopologyType;

/* granny_type_listing.h bindings: */
GRANNY_DYNLINK(granny_int32x) GrannyGetDefinedTypeCount(void);
extern GRANNY_DYNLINKDATA(granny_defined_type *) GrannyDefinedTypes;

/* granny_variant_builder.h bindings: */
GRANNY_DYNLINK(granny_variant_builder *) GrannyBeginVariant(granny_string_table * StringTableBuilder);
GRANNY_DYNLINK(bool) GrannyEndVariant(granny_variant_builder * Builder,
                                      granny_data_type_definition ** Type,
                                      void ** Object);
GRANNY_DYNLINK(void) GrannyAbortVariant(granny_variant_builder * Builder);
GRANNY_DYNLINK(granny_int32x) GrannyGetResultingVariantTypeSize(granny_variant_builder const * Builder);
GRANNY_DYNLINK(granny_int32x) GrannyGetResultingVariantObjectSize(granny_variant_builder const * Builder);
GRANNY_DYNLINK(bool) GrannyEndVariantInPlace(granny_variant_builder * Builder,
                                             void * TypeMemory,
                                             granny_data_type_definition ** Type,
                                             void * ObjectMemory,
                                             void ** Object);
GRANNY_DYNLINK(void) GrannyAddBoolMember(granny_variant_builder * Builder,
                                         char const * Name,
                                         granny_bool32 Value);
GRANNY_DYNLINK(void) GrannyAddIntegerMember(granny_variant_builder * Builder,
                                            char const * Name,
                                            granny_int32 Value);
GRANNY_DYNLINK(void) GrannyAddIntegerArrayMember(granny_variant_builder * Builder,
                                                 char const * Name,
                                                 granny_int32x Width,
                                                 granny_int32 *  Value);
GRANNY_DYNLINK(void) GrannyAddUnsignedIntegerMember(granny_variant_builder * Builder,
                                                    char const * Name,
                                                    granny_uint32 Value);
GRANNY_DYNLINK(void) GrannyAddScalarMember(granny_variant_builder * Builder,
                                           char const * Name,
                                           granny_real32 Value);
GRANNY_DYNLINK(void) GrannyAddScalarArrayMember(granny_variant_builder * Builder,
                                                char const * Name,
                                                granny_int32x Width,
                                                granny_real32 const * Value);
GRANNY_DYNLINK(void) GrannyAddStringMember(granny_variant_builder * Builder,
                                           char const * Name,
                                           char const * Value);
GRANNY_DYNLINK(void) GrannyAddReferenceMember(granny_variant_builder * Builder,
                                              char const * Name,
                                              granny_data_type_definition * Type,
                                              void * Value);
GRANNY_DYNLINK(void) GrannyAddDynamicArrayMember(granny_variant_builder * Builder,
                                                 char const * Name,
                                                 granny_int32x Count,
                                                 granny_data_type_definition * EntryType,
                                                 void * ArrayEntries);

/* granny_version_checking.h bindings: */
GRANNY_DYNLINK(char const *) GrannyGetVersionString(void);
GRANNY_DYNLINK(void) GrannyGetVersion(granny_int32x * MajorVersion,
                                      granny_int32x * MinorVersion,
                                      granny_int32x * Customization,
                                      granny_int32x * BuildNumber);
GRANNY_DYNLINK(bool) GrannyVersionsMatch_(granny_int32x MajorVersion,
                                          granny_int32x MinorVersion,
                                          granny_int32x Customization,
                                          granny_int32x BuildNumber);

/* granny_vertex_data.h bindings: */
GRANNY_DYNLINK(void) GrannyConvertVertexLayouts(granny_int32x VertexCount,
                                                granny_data_type_definition const * SourceLayout,
                                                void const * SourceVertices,
                                                granny_data_type_definition const * DestLayout,
                                                void * DestVertices);
GRANNY_DYNLINK(void) GrannyEnsureExactOneNorm(granny_data_type_definition const * WeightsType,
                                              void * WeightsInit);
GRANNY_DYNLINK(void) GrannyOneNormalizeWeights(granny_int32x VertexCount,
                                               granny_data_type_definition const * Layout,
                                               void * Vertices);
GRANNY_DYNLINK(void) GrannyTransformVertices(granny_int32x VertexCount,
                                             granny_data_type_definition const * Layout,
                                             void * Vertices,
                                             granny_real32 const * Affine3,
                                             granny_real32 const * Linear3x3,
                                             granny_real32 const * InverseLinear3x3,
                                             bool Renormalize,
                                             bool TreatAsDeltas);
GRANNY_DYNLINK(void) GrannyNormalizeVertices(granny_int32x VertexCount,
                                             granny_data_type_definition const * LayoutType,
                                             void * Vertices);
GRANNY_DYNLINK(granny_int32x) GrannyGetVertexTextureCoordinatesName(granny_int32x Index,
                                                                    char * Buffer);
GRANNY_DYNLINK(granny_int32x) GrannyGetVertexDiffuseColorName(granny_int32x Index,
                                                              char * Buffer);
GRANNY_DYNLINK(granny_int32x) GrannyGetVertexSpecularColorName(granny_int32x Index,
                                                               char * Buffer);
GRANNY_DYNLINK(bool) GrannyIsSpatialVertexMember(char const * Name);
GRANNY_DYNLINK(granny_int32x) GrannyGetVertexBoneCount(granny_data_type_definition const * VertexType);
GRANNY_DYNLINK(granny_int32x) GrannyGetVertexChannelCount(granny_data_type_definition const * VertexType);
GRANNY_DYNLINK(void) GrannyGetSingleVertex(granny_vertex_data const * VertexData,
                                           granny_int32x VertexIndex,
                                           granny_data_type_definition const * As,
                                           void * Dest);
GRANNY_DYNLINK(void) GrannySetVertexPosition(granny_data_type_definition const * VertexLayout,
                                             void * VertexPointer,
                                             granny_real32 const * Position);
GRANNY_DYNLINK(void) GrannySetVertexNormal(granny_data_type_definition const * VertexLayout,
                                           void * VertexPointer,
                                           granny_real32 const * Normal);
GRANNY_DYNLINK(void) GrannySetVertexColor(granny_data_type_definition const  * VertexLayout,
                                          void * VertexPointer,
                                          granny_int32x ColorIndex,
                                          granny_real32 const * Color);
GRANNY_DYNLINK(void) GrannySetVertexUVW(granny_data_type_definition const * VertexLayout,
                                        void * VertexPointer,
                                        granny_int32x UVWIndex,
                                        granny_real32 const * UVW);
GRANNY_DYNLINK(granny_int32x) GrannyGetVertexComponentCount(granny_data_type_definition const * VertexLayout);
GRANNY_DYNLINK(granny_int32x) GrannyGetVertexComponentIndex(char const * ComponentName,
                                                            granny_data_type_definition const * VertexLayout);
GRANNY_DYNLINK(char const * ) GrannyGetVertexComponentToolName(char const * ComponentName,
                                                               granny_vertex_data const * VertexData);
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyVertexAnnotationSetType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyVertexDataType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyP3VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPT32VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPN33VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPNG333VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPNGT3332VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPNTG3323VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPNGB3333VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPNT332VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPNGBT33332VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPNT333VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPNGBT33333VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPWN313VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPWNG3133VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPWNGT31332VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPWNGB31333VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPWNT3132VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPWNGBT313332VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPWN323VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPWNG3233VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPWNGT32332VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPWNGB32333VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPWNT3232VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPWNGBT323332VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPWN343VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPWNG3433VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPWNGT34332VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPWNGB34333VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPWNT3432VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyPWNGBT343332VertexType;
extern GRANNY_DYNLINKDATA(granny_data_type_definition *) GrannyVertexWeightArraysType;

/* granny_world_pose.h bindings: */
GRANNY_DYNLINK(granny_world_pose *) GrannyNewWorldPose(granny_int32x BoneCount);
GRANNY_DYNLINK(granny_world_pose *) GrannyNewWorldPoseNoComposite(granny_int32x BoneCount);
GRANNY_DYNLINK(void) GrannyFreeWorldPose(granny_world_pose * WorldPose);
GRANNY_DYNLINK(granny_int32x) GrannyGetWorldPoseBoneCount(granny_world_pose const * WorldPose);
GRANNY_DYNLINK(granny_int32x) GrannyGetResultingWorldPoseSize(granny_int32x BoneCount,
                                                              granny_composite_flag CompositeFlag);
GRANNY_DYNLINK(granny_world_pose *) GrannyNewWorldPoseInPlace(granny_int32x BoneCount,
                                                              granny_composite_flag CompositeFlag,
                                                              void * Memory);
GRANNY_DYNLINK(granny_real32 *) GrannyGetWorldPose4x4(granny_world_pose const * WorldPose,
                                                      granny_int32x BoneIndex);
GRANNY_DYNLINK(granny_real32 *) GrannyGetWorldPoseComposite4x4(granny_world_pose const * WorldPose,
                                                               granny_int32x BoneIndex);
GRANNY_DYNLINK(granny_matrix_4x4 *) GrannyGetWorldPose4x4Array(granny_world_pose const * WorldPose);
GRANNY_DYNLINK(granny_matrix_4x4 *) GrannyGetWorldPoseComposite4x4Array(granny_world_pose const * WorldPose);
GRANNY_DYNLINK(void) GrannyBuildWorldPose(granny_skeleton const * Skeleton,
                                          granny_int32x FirstBone,
                                          granny_int32x BoneCount,
                                          granny_local_pose const * LocalPose,
                                          granny_real32 const * Offset4x4,
                                          granny_world_pose * Result);
GRANNY_DYNLINK(void) GrannyBuildWorldPoseLOD(granny_skeleton const * Skeleton,
                                             granny_int32x FirstBone,
                                             granny_int32x BoneCount,
                                             granny_int32x FirstValidLocalBone,
                                             granny_int32x ValidLocalBoneCount,
                                             granny_local_pose const * LocalPose,
                                             granny_real32 const * Offset4x4,
                                             granny_world_pose * Result);
GRANNY_DYNLINK(void) GrannyBuildWorldPoseSparse(granny_skeleton const * Skeleton,
                                                granny_int32x FirstBone,
                                                granny_int32x BoneCount,
                                                granny_int32x const * SparseBoneArray,
                                                granny_int32x const * SparseBoneArrayReverse,
                                                granny_local_pose const * LocalPose,
                                                granny_real32 const * Offset4x4,
                                                granny_world_pose * Result);
GRANNY_DYNLINK(void) GrannyBuildRestWorldPose(granny_skeleton const * Skeleton,
                                              granny_int32x FirstBone,
                                              granny_int32x BoneCount,
                                              granny_real32 const * Offset4x4,
                                              granny_world_pose * Result);
GRANNY_DYNLINK(void) GrannyBuildWorldPoseNoComposite(granny_skeleton const * Skeleton,
                                                     granny_int32x FirstBone,
                                                     granny_int32x BoneCount,
                                                     granny_local_pose const * LocalPose,
                                                     granny_real32 const * Offset4x4,
                                                     granny_world_pose * Result);
GRANNY_DYNLINK(void) GrannyBuildWorldPoseNoCompositeLOD(granny_skeleton const * Skeleton,
                                                        granny_int32x FirstBone,
                                                        granny_int32x BoneCount,
                                                        granny_int32x FirstValidLocalBone,
                                                        granny_int32x ValidLocalBoneCount,
                                                        granny_local_pose const * LocalPose,
                                                        granny_real32 const * Offset4x4,
                                                        granny_world_pose * Result);
GRANNY_DYNLINK(void) GrannyBuildWorldPoseNoCompositeSparse(granny_skeleton const * Skeleton,
                                                           granny_int32x FirstBone,
                                                           granny_int32x BoneCount,
                                                           granny_int32x const * SparseBoneArray,
                                                           granny_int32x const * SparseBoneArrayReverse,
                                                           granny_local_pose const * LocalPose,
                                                           granny_real32 const * Offset4x4,
                                                           granny_world_pose * Result);
GRANNY_DYNLINK(void) GrannyBuildWorldPoseComposites(granny_skeleton const * Skeleton,
                                                    granny_int32x FirstBone,
                                                    granny_int32x BoneCount,
                                                    granny_world_pose * Result);
GRANNY_DYNLINK(void) GrannyBuildCompositeBuffer(granny_skeleton const * Skeleton,
                                                granny_int32x FirstBone,
                                                granny_int32x BoneCount,
                                                granny_world_pose const * Pose,
                                                granny_matrix_4x4 *  CompositeBuffer);
GRANNY_DYNLINK(void) GrannyBuildCompositeBufferTransposed(granny_skeleton const * Skeleton,
                                                          granny_int32x FirstBone,
                                                          granny_int32x BoneCount,
                                                          granny_world_pose const * Pose,
                                                          granny_matrix_3x4 *  CompositeBuffer);
GRANNY_DYNLINK(void) GrannyBuildIndexedCompositeBuffer(granny_skeleton const * Skeleton,
                                                       granny_world_pose const * Pose,
                                                       granny_int32x const*  Indices,
                                                       granny_int32x IndexCount,
                                                       granny_matrix_4x4 *  CompositeBuffer);
GRANNY_DYNLINK(void) GrannyBuildIndexedCompositeBufferTransposed(granny_skeleton const * Skeleton,
                                                                 granny_world_pose const * Pose,
                                                                 granny_int32x const*  Indices,
                                                                 granny_int32x IndexCount,
                                                                 granny_matrix_3x4 *  CompositeBuffer);
GRANNY_DYNLINK(void) GrannyUpdateWorldPoseChildren(granny_skeleton const * Skeleton,
                                                   granny_int32x ParentBone,
                                                   granny_local_pose * LocalPose,
                                                   granny_real32 const * Offset4x4,
                                                   granny_world_pose * Result);
#ifdef __cplusplus
}
#endif

// Don't export this macro...
#undef GrannyTypeSizeCheck


// Put the packing state back where it was, and undefine our
//  attribute macro...
#ifdef _MSC_VER
  #pragma pack(pop)
  #pragma warning(pop)
#endif

#undef GrannyAlignAttribute



#if defined(__MWERKS__)
  #pragma export off
#endif

#define GRANNY_H
#endif
