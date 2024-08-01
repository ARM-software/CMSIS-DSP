// -*- C++ -*-
/** @file */ 
#pragma once 

/** \addtogroup FUSION 
 *  @{
 */

/**
 * @brief  Unary operator
 *
 * @tparam Scalar Datatype for scalar
 * @tparam Derived Datatype representing the operator expression
 * 
 */
template<typename Scalar,typename Derived>
struct _UnaryOperator{
    Derived& derived()  {return(static_cast<Derived&>(*this));}

    Derived const& derived() const {return(static_cast<Derived const&>(*this));}

    Scalar const operator()(const Scalar lhs) const
    {
        return(this->derived()(lhs));
    }

    #if defined(HAS_VECTOR)
    using Vector= typename vector_traits<Scalar>::vector ;
    using pred_t = typename vector_traits<Scalar>::predicate_t;

    Vector const operator()(const Vector lhs) const
    {
        return(this->derived()(lhs));
    }

    /*

    Predicated operation when exists (Helium)

    */
    template<typename T=Scalar,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    Vector const operator()(const Vector lhs,const pred_t p0) const
    {
        return(this->derived()(lhs,p0));
    }

    /*
    Vector const to_vector(const Scalar lhs) const
    {
        return(this->derived().to_vector(lhs));
    }
    */
#endif
};

template<typename Scalar,typename Derived>
struct DualType<_UnaryOperator<Scalar,Derived>>
{
   using Scalar_D = typename DualType<Scalar>::type;
   using Derived_D = typename DualType<Derived>::type;

   typedef _UnaryOperator<Scalar_D,Derived_D> type;
};

/**
 * @brief  Unary operator
 *
 * @tparam Scalar Datatype for scalar
 * @tparam Derived Datatype representing the operator expression
 * 
 */
template<typename ScalarLHS,typename ScalarRHS,typename Derived>
struct _BinaryOperator{
    Derived& derived()  {return(static_cast<Derived&>(*this));}

    Derived const& derived() const {return(static_cast<Derived const&>(*this));}

    auto  operator()(const ScalarLHS lhs, 
                     const ScalarRHS rhs) const
    {
        return(this->derived()(lhs,rhs));
    }

    #if defined(HAS_VECTOR)
    using VectorLHS = typename vector_traits<ScalarLHS>::vector ;
    using VectorRHS = typename vector_traits<ScalarRHS>::vector ;
    using pred_t = typename vector_traits<ScalarLHS>::predicate_t;


    auto  operator()(const VectorLHS lhs, 
                     const VectorRHS rhs) const
    {
        return(this->derived()(lhs,rhs));
    }

    auto  operator()(const VectorLHS lhs, 
                     const ScalarRHS rhs) const
    {
        return(this->derived()(lhs,rhs));
    }

    auto  operator()(const ScalarLHS lhs, 
                     const VectorRHS rhs) const
    {
        return(this->derived()(lhs,rhs));
    }

    template<typename T=ScalarLHS,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    auto  operator()(const VectorLHS lhs, 
                     const VectorRHS rhs,
                     const pred_t p0) const
    {
        return(this->derived()(lhs,rhs,p0));
    }

    template<typename T=ScalarLHS,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    auto  operator()(const VectorLHS lhs, 
                     const ScalarRHS rhs,
                     const pred_t p0) const
    {
        return(this->derived()(lhs,rhs,p0));
    }

    template<typename T=ScalarLHS,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    auto  operator()(const ScalarLHS lhs, 
                     const VectorRHS rhs,
                     const pred_t p0) const
    {
        return(this->derived()(lhs,rhs,p0));
    }
#endif
};


template<typename ScalarLHS,typename ScalarRHS, typename Derived>
struct DualType<_BinaryOperator<ScalarLHS,ScalarRHS,Derived>>
{
   using ScalarLHS_D = typename DualType<ScalarLHS>::type;
   using ScalarRHS_D = typename DualType<ScalarRHS>::type;
   using Derived_D = typename DualType<Derived>::type;

   typedef _BinaryOperator<ScalarLHS_D,ScalarRHS_D,Derived_D> type;
};

/*
 * 
 * BINARY
 * 
 */

/**
 * @brief  Add operator
 *
 * @tparam Scalar Datatype for scalar
 * 
 */
template<typename ScalarLHS,typename ScalarRHS>
struct _AddOp:_BinaryOperator<ScalarLHS,ScalarRHS,_AddOp<ScalarLHS,ScalarRHS>>
{
    auto  operator()(const ScalarLHS lhs, 
                     const ScalarRHS rhs) const {
        return(lhs + rhs);
    }

#if defined(HAS_VECTOR)
    using VectorLHS = typename vector_traits<ScalarLHS>::vector ;
    using VectorRHS = typename vector_traits<ScalarRHS>::vector ;
    using pred_t = typename vector_traits<ScalarLHS>::predicate_t;

    auto  operator()(const VectorLHS lhs, 
                     const VectorRHS rhs) const
    {
        return(inner::vadd(lhs,rhs));
    }

    auto  operator()(const VectorLHS lhs, 
                     const ScalarRHS rhs) const
    {
        return(inner::vadd(lhs,rhs));
    }

    auto  operator()(const ScalarLHS lhs, 
                     const VectorRHS rhs) const
    {
        return(inner::vadd(lhs,rhs));
    }

    template<typename T=ScalarLHS,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    auto  operator()(const VectorLHS lhs, 
                     const VectorRHS rhs,
                     const pred_t p0) const
    {
        return(inner::vadd(lhs,rhs,p0));
    }

    template<typename T=ScalarLHS,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    auto  operator()(const VectorLHS lhs, 
                     const ScalarRHS rhs,
                     const pred_t p0) const
    {
        return(inner::vadd(lhs,rhs,p0));
    }

    template<typename T=ScalarLHS,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    auto  operator()(const ScalarLHS lhs, 
                     const VectorRHS rhs,
                     const pred_t p0) const
    {
        return(inner::vadd(lhs,rhs,p0));
    }
#endif
};

template<typename ScalarLHS,typename ScalarRHS>
struct DualType<_AddOp<ScalarLHS,ScalarRHS>>
{
   using ScalarLHS_D = typename DualType<ScalarLHS>::type;
   using ScalarRHS_D = typename DualType<ScalarRHS>::type;

   typedef _AddOp<ScalarLHS_D,ScalarRHS_D> type;
};

/**
 * @brief  Sub operator
 *
 * @tparam Scalar Datatype for scalar
 * 
 */
template<typename ScalarLHS, typename ScalarRHS>
struct _SubOp:_BinaryOperator<ScalarLHS,ScalarRHS,_SubOp<ScalarLHS,ScalarRHS>>
{
    auto  operator()(const ScalarLHS lhs, 
                     const ScalarRHS rhs) const {
        return(lhs - rhs);
    }

#if defined(HAS_VECTOR)
    using VectorLHS = typename vector_traits<ScalarLHS>::vector ;
    using VectorRHS = typename vector_traits<ScalarRHS>::vector ;
    using pred_t = typename vector_traits<ScalarLHS>::predicate_t;

    auto  operator()(const VectorLHS lhs, 
                     const VectorRHS rhs) const
    {
        return(inner::vsub(lhs,rhs));
    }

    auto  operator()(const VectorLHS lhs, 
                     const ScalarRHS rhs) const
    {
        return(inner::vsub(lhs,rhs));
    }

    auto  operator()(const ScalarLHS lhs, 
                     const VectorRHS rhs) const
    {
        return(inner::vsub(lhs,rhs));
    }

    template<typename T=ScalarLHS,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    auto  operator()(const VectorLHS lhs, 
                     const VectorRHS rhs,
                     const pred_t p0) const
    {
        return(inner::vsub(lhs,rhs,p0));
    }

    template<typename T=ScalarLHS,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    auto  operator()(const VectorLHS lhs, 
                     const ScalarRHS rhs,
                     const pred_t p0) const
    {
        return(inner::vsub(lhs,rhs,p0));
    }

    template<typename T=ScalarLHS,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    auto  operator()(const ScalarLHS lhs, 
                     const VectorRHS rhs,
                     const pred_t p0) const
    {
        return(inner::vsub(lhs,rhs,p0));
    }
#endif
};

template<typename ScalarLHS,typename ScalarRHS>
struct DualType<_SubOp<ScalarLHS,ScalarRHS>>
{
   using ScalarLHS_D = typename DualType<ScalarLHS>::type;
   using ScalarRHS_D = typename DualType<ScalarRHS>::type;

   typedef _SubOp<ScalarLHS_D,ScalarRHS_D> type;
};

/**
 * @brief  Mul operator
 *
 * @tparam Scalar Datatype for scalar
 * 
 */
template<typename ScalarLHS,typename ScalarRHS>
struct _MulOp:_BinaryOperator<ScalarLHS,ScalarRHS,_MulOp<ScalarLHS,ScalarRHS>>
{
    auto  operator()(const ScalarLHS lhs, 
                     const ScalarRHS rhs) const {
        return(lhs * rhs);
    }

#if defined(HAS_VECTOR)
    using VectorLHS = typename vector_traits<ScalarLHS>::vector ;
    using VectorRHS = typename vector_traits<ScalarRHS>::vector ;

    using pred_t = typename vector_traits<ScalarLHS>::predicate_t;

    auto  operator()(const VectorLHS lhs, 
                     const VectorRHS rhs) const
    {
        return(inner::vmul(lhs,rhs));
    }

    auto operator()(const VectorLHS lhs, 
                    const ScalarRHS rhs) const
    {
        return(inner::vmul(lhs,rhs));
    }

    auto operator()(const ScalarLHS lhs, 
                    const VectorRHS rhs) const
    {
        return(inner::vmul(lhs,rhs));
    }

    template<typename T=ScalarLHS,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    auto operator()(const VectorLHS lhs, 
                    const VectorRHS rhs,
                    const pred_t p0) const
    {
        return(inner::vmul(lhs,rhs,p0));
    }

    template<typename T=ScalarLHS,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    auto  operator()(const VectorLHS lhs, 
                     const ScalarRHS rhs,
                     const pred_t p0) const
    {
        return(inner::vmul(lhs,rhs,p0));
    }

    template<typename T=ScalarLHS,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    auto  operator()(const ScalarLHS lhs, 
                     const VectorRHS rhs,
                     const pred_t p0) const
    {
        return(inner::vmul(lhs,rhs,p0));
    }
#endif
};

template<typename ScalarLHS,typename ScalarRHS>
struct DualType<_MulOp<ScalarLHS,ScalarRHS>>
{
   using ScalarLHS_D = typename DualType<ScalarLHS>::type;
   using ScalarRHS_D = typename DualType<ScalarRHS>::type;

   typedef _MulOp<ScalarLHS_D,ScalarRHS_D> type;
};

/*
 * 
 * UNARY
 * 
 */

/**
 * @brief  Neg operator
 *
 * @tparam Scalar Datatype for scalar
 * 
 */
template<typename Scalar>
struct _NegOp:_UnaryOperator<Scalar,_NegOp<Scalar>>
{
    Scalar const operator()(const Scalar lhs) const {
        return(-lhs);
    }

#if defined(HAS_VECTOR)
    using Vector= typename vector_traits<Scalar>::vector ;
    using pred_t = typename vector_traits<Scalar>::predicate_t;

    Vector const operator()(const Vector lhs) const
    {
        return(inner::vneg(lhs));
    }

    template<typename T=Scalar,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    Vector const operator()(const Vector lhs,
                            const pred_t p0) const
    {
        return(inner::vneg(lhs,p0));
    }

   
#endif
};

template<typename Scalar>
struct DualType<_NegOp<Scalar>>
{
   using Scalar_D = typename DualType<Scalar>::type;

   typedef _NegOp<Scalar_D> type;
};

/**
 * @brief  No operator
 *
 * @tparam Scalar Datatype for scalar
 * 
 */
template<typename Scalar>
struct _NoOp:_UnaryOperator<Scalar,_NoOp<Scalar>>
{
    Scalar const operator()(const Scalar lhs) const {
        return(lhs);
    }

#if defined(HAS_VECTOR)
    using Vector= typename vector_traits<Scalar>::vector ;
    using pred_t = typename vector_traits<Scalar>::predicate_t;

    Vector const operator()(const Vector lhs) const
    {
        return(lhs);
    }

    template<typename T=Scalar,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    Vector const operator()(const Vector lhs,
                            const pred_t p0) const
    {
        (void)p0;
        return(lhs);
    }

#endif
};

template<typename Scalar>
struct DualType<_NoOp<Scalar>>
{
   using Scalar_D = typename DualType<Scalar>::type;

   typedef _NoOp<Scalar_D> type;
};

/**
 * @brief  Conjugate operator
 *
 * @tparam Scalar Datatype for scalar
 * 
 */
template<typename Scalar>
struct _ConjugateOp:_UnaryOperator<Scalar,_ConjugateOp<Scalar>>
{

    template<typename T=Scalar,
             typename std::enable_if<IsComplexNumber<T>::value,bool>::type = true>
    Scalar const operator()(const Scalar lhs) const {
        return(std::conj(lhs));
    }

    template<typename T=Scalar,
             typename std::enable_if<!IsComplexNumber<T>::value,bool>::type = true>
    Scalar const operator()(const Scalar lhs) const {
        return(lhs);
    }

#if defined(HAS_VECTOR)

    using Vector= typename vector_traits<Scalar>::vector ;
    using pred_t = typename vector_traits<Scalar>::predicate_t;

    Vector const operator()(const Vector lhs) const
    {
        return(inner::vconjugate(lhs));
    }

    template<typename T=Scalar,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    Vector const operator()(const Vector lhs,
                            const pred_t p0) const
    {
        return(inner::vconjugate(lhs,p0));
    }

 
#endif
};

template<typename Scalar>
struct DualType<_ConjugateOp<Scalar>>
{
   using Scalar_D = typename DualType<Scalar>::type;

   typedef _ConjugateOp<Scalar_D> type;
};

/*! @} */