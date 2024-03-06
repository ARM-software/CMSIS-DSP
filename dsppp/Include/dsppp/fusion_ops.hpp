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

/**
 * @brief  Unary operator
 *
 * @tparam Scalar Datatype for scalar
 * @tparam Derived Datatype representing the operator expression
 * 
 */
template<typename Scalar,typename Derived>
struct _BinaryOperator{
    Derived& derived()  {return(static_cast<Derived&>(*this));}

    Derived const& derived() const {return(static_cast<Derived const&>(*this));}

    Scalar const operator()(const Scalar lhs, 
                            const Scalar rhs) const
    {
        return(this->derived()(lhs,rhs));
    }

    #if defined(HAS_VECTOR)
    using Vector= typename vector_traits<Scalar>::vector ;
    using pred_t = typename vector_traits<Scalar>::predicate_t;


    Vector const operator()(const Vector lhs, 
                            const Vector rhs) const
    {
        return(this->derived()(lhs,rhs));
    }

    Vector const operator()(const Vector lhs, 
                            const Scalar rhs) const
    {
        return(this->derived()(lhs,rhs));
    }

    Vector const operator()(const Scalar lhs, 
                            const Vector rhs) const
    {
        return(this->derived()(lhs,rhs));
    }

    template<typename T=Scalar,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    Vector const operator()(const Vector lhs, 
                            const Vector rhs,
                            const pred_t p0) const
    {
        return(this->derived()(lhs,rhs,p0));
    }

    template<typename T=Scalar,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    Vector const operator()(const Vector lhs, 
                            const Scalar rhs,
                            const pred_t p0) const
    {
        return(this->derived()(lhs,rhs,p0));
    }

    template<typename T=Scalar,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    Vector const operator()(const Scalar lhs, 
                            const Vector rhs,
                            const pred_t p0) const
    {
        return(this->derived()(lhs,rhs,p0));
    }
#endif
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
template<typename Scalar>
struct _AddOp:_BinaryOperator<Scalar,_AddOp<Scalar>>
{
    Scalar const operator()(const Scalar lhs, 
                            const Scalar rhs) const {
        return(lhs + rhs);
    }

#if defined(HAS_VECTOR)
    using Vector=typename vector_traits<Scalar>::vector ;
    using pred_t = typename vector_traits<Scalar>::predicate_t;

    Vector const operator()(const Vector lhs, 
                            const Vector rhs) const
    {
        return(inner::vadd(lhs,rhs));
    }

    Vector const operator()(const Vector lhs, 
                            const Scalar rhs) const
    {
        return(inner::vadd(lhs,rhs));
    }

    Vector const operator()(const Scalar lhs, 
                            const Vector rhs) const
    {
        return(inner::vadd(lhs,rhs));
    }

    template<typename T=Scalar,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    Vector const operator()(const Vector lhs, 
                            const Vector rhs,
                            const pred_t p0) const
    {
        return(inner::vadd(lhs,rhs,p0));
    }

    template<typename T=Scalar,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    Vector const operator()(const Vector lhs, 
                            const Scalar rhs,
                            const pred_t p0) const
    {
        return(inner::vadd(lhs,rhs,p0));
    }

    template<typename T=Scalar,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    Vector const operator()(const Scalar lhs, 
                            const Vector rhs,
                            const pred_t p0) const
    {
        return(inner::vadd(lhs,rhs,p0));
    }
#endif
};

/**
 * @brief  Sub operator
 *
 * @tparam Scalar Datatype for scalar
 * 
 */
template<typename Scalar>
struct _SubOp:_BinaryOperator<Scalar,_SubOp<Scalar>>
{
    Scalar const operator()(const Scalar lhs, 
                            const Scalar rhs) const {
        return(lhs - rhs);
    }

#if defined(HAS_VECTOR)
    using Vector=typename vector_traits<Scalar>::vector ;
    using pred_t = typename vector_traits<Scalar>::predicate_t;

    Vector const operator()(const Vector lhs, 
                            const Vector rhs) const
    {
        return(inner::vsub(lhs,rhs));
    }

    Vector const operator()(const Vector lhs, 
                            const Scalar rhs) const
    {
        return(inner::vsub(lhs,rhs));
    }

    Vector const operator()(const Scalar lhs, 
                            const Vector rhs) const
    {
        return(inner::vsub(lhs,rhs));
    }

    template<typename T=Scalar,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    Vector const operator()(const Vector lhs, 
                            const Vector rhs,
                            const pred_t p0) const
    {
        return(inner::vsub(lhs,rhs,p0));
    }

    template<typename T=Scalar,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    Vector const operator()(const Vector lhs, 
                            const Scalar rhs,
                            const pred_t p0) const
    {
        return(inner::vsub(lhs,rhs,p0));
    }

    template<typename T=Scalar,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    Vector const operator()(const Scalar lhs, 
                            const Vector rhs,
                            const pred_t p0) const
    {
        return(inner::vsub(lhs,rhs,p0));
    }
#endif
};


/**
 * @brief  Mul operator
 *
 * @tparam Scalar Datatype for scalar
 * 
 */
template<typename Scalar>
struct _MulOp:_BinaryOperator<Scalar,_MulOp<Scalar>>
{
    Scalar const operator()(const Scalar lhs, 
                            const Scalar rhs) const {
        return(lhs * rhs);
    }

#if defined(HAS_VECTOR)
    using Vector= typename vector_traits<Scalar>::vector ;
    using pred_t = typename vector_traits<Scalar>::predicate_t;

    Vector const operator()(const Vector lhs, 
                            const Vector rhs) const
    {
        return(inner::vmul(lhs,rhs));
    }

    Vector const operator()(const Vector lhs, 
                            const Scalar rhs) const
    {
        return(inner::vmul(lhs,rhs));
    }

    Vector const operator()(const Scalar lhs, 
                            const Vector rhs) const
    {
        return(inner::vmul(lhs,rhs));
    }

    template<typename T=Scalar,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    Vector const operator()(const Vector lhs, 
                            const Vector rhs,
                            const pred_t p0) const
    {
        return(inner::vmul(lhs,rhs,p0));
    }

    template<typename T=Scalar,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    Vector const operator()(const Vector lhs, 
                            const Scalar rhs,
                            const pred_t p0) const
    {
        return(inner::vmul(lhs,rhs,p0));
    }

    template<typename T=Scalar,
             typename std::enable_if<vector_traits<T>::has_predicate,bool>::type = true>
    Vector const operator()(const Scalar lhs, 
                            const Vector rhs,
                            const pred_t p0) const
    {
        return(inner::vmul(lhs,rhs,p0));
    }
#endif
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

/*! @} */