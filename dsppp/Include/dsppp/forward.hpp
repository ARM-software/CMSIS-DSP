// -*- C++ -*-
/** @file */ 
#pragma once 

namespace arm_cmsis_dsp {

template<typename P>
struct Vector_Base;

template<typename T,int stride>
struct VectorView;

template<typename P,int L,
         template<int> typename Allocator>
struct Vector;

template<typename P,int R,int C,
         template<int> typename Allocator>
struct Matrix;

template<typename T,int S>
struct MatrixView;

template<typename T>
struct NbRows;

template<typename T>
struct NbCols;

template<typename M>
struct Complexity;

template<typename M>
struct OutputVectorDim;

template<typename M,typename V>
struct CompatibleStaticMatMatProduct;

template<typename M,typename V>
struct CompatibleStaticMatVecProduct;

template<typename M,typename V>
struct CompatibleDynamicMatVecProduct;

template<typename M,typename V>
struct CompatibleDynamicMatMatProductStaticStride;

template<typename M,typename V>
struct CompatibleDynamicMatMatProductDynamicStride;

template<typename M,typename V>
struct CompatibleDynamicMatMatProduct;

template<typename M,typename V>
struct OutputVector;

template<typename MA,typename MB>
struct OutputMatrix;


/*

Identifications

*/

/* 

Is a contiguous array in memory with scalar indexing
(operator[])
It can be an _Expr
Vector has a length

Generally used when scalar indexing is required or length

*/
template<typename T>
struct IsVector;

/*

Has matrix indexing (operator())
and matrix operations like transpose, identity.
So it cannot be an _Expr because _Expr has no transpose, identity
Has rows, columns
Matrix may be vectors Vectors (with above definition)

Generally used when transpose or identity are required.

*/
template<typename T>
struct IsMatrix;

/*

Has matrix indexing (operator())
but no matrix operator like transpose.
It can be an Expr
Has rows, columns
It may not always be a Vector (MatrixView are not contiguous)

Generally used only when matrix indexing is mandatory

*/
template<typename T>
struct HasMatrixIndexing;

/*


Type Matrix                     : IsVector, IsMatrix, HasMatrixIndexing
Type MatrixView                 :         , IsMatrix, HasMatrixIndexing
Type _Expr with Matrix          : IsVector,         , HasMatrixIndexing 
Type _Expr with some MatrixView :                     HasMatrixIndexing

*/


/*

Dimensions only known at runtime

*/
template<typename T>
struct IsDynamic;

/*

StaticLength if known at build time otherwise 0
*/
template<typename T>
struct StaticLength;

/*

Type of elements in vector or matrix

*/
template<typename T>
struct ElementType;

template<typename T>
struct IsComplexNumber;

template<typename T>
struct ComplexNumberType;

template<typename T>
struct HasStaticStride;

template<typename T>
struct StaticStride;

}
