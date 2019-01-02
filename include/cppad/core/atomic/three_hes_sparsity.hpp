# ifndef CPPAD_CORE_ATOMIC_THREE_HES_SPARSITY_HPP
# define CPPAD_CORE_ATOMIC_THREE_HES_SPARSITY_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin atomic_three_hes_sparsity$$
$spell
    Hessian
    afun
    hes
$$

$section Atomic Function Hessian Sparsity Patterns$$

$head Syntax$$
$icode%ok% = %afun%.hes_sparsity(
    %parameter_x%, %select_x%, %select_y%, %pattern_out%
)%$$

$head Prototype$$
$srcfile%include/cppad/core/atomic/three_hes_sparsity.hpp
    %0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1
%$$

$head Implementation$$
This function must be defined if
$cref/afun/atomic_three_ctor/atomic_user/afun/$$ is
used to define an $cref ADFun$$ object $icode f$$,
and Hessian sparsity patterns are computed for $icode f$$.

$head parameter_x$$
This argument contains the value of the parameters in
$cref/ax/atomic_three_afun/ax/$$ for the corresponding call to
$codei%
    %afun%(%ax%, %ay%)
%$$
If the vector $icode ax$$ represented a matrix,
$icode%ax%[0]%$$ could be the number of rows in the matrix.
Note that the value of dynamic parameters may have changed since
the call above; see $cref new_dynamic$$.
It $icode%ax%[%j%]%$$ is a variable,
$icode%parameter_x[%j%]%$$ is $code nan$$.

$head select_x$$
This argument has size equal to the number of arguments to this
atomic function; i.e. the size of $icode ax$$.
It specifies which domain components are included in
the calculation of $icode pattern_out$$.
If $icode%select_x%[%j%]%$$ is false, then there will be no indices
$icode k$$ such that either of the following hold:
$codei%
    %pattern_out%.row()[%k%] == %j%
    %pattern_out%.col()[%k%] == %j%
%$$.

$head select_y$$
This argument has size equal to the number of results to this
atomic function; i.e. the size of $icode ay$$.
It specifies which range component functions $latex g_i (x)$$ are included in
of $icode pattern_out$$.

$head pattern_out$$
This input value of $icode pattern_out$$ does not matter.
Upon return it is the union,
with respect to $icode i$$ such that $icode%select_y%[%i%]%$$ is true,
of the sparsity pattern for the lower triangle of the Hessian of
$latex g_i (x)$$.
To be specific, there are non-negative indices
$icode i$$, $icode r$$, $icode c$$, and $icode k$$ such that
$codei%
    %pattern_out%.row()[%k%] == %r%
    %pattern_out%.col()[%k%] == %c%
%$$
if and only if
$icode%select_y%[%i%]%$$ is true,
$icode%select_x%[%r%]%$$ is true,
$icode%select_x%[%c%]%$$ is true,
$icode%c% <= %r%$$, and
$latex \[
    \partial_{x(r)} \partial_{x(c)} g_i(x)
\] $$
is possibly non-zero.

$head ok$$
If this calculation succeeded, $icode ok$$ is true.
Otherwise it is false.

$end
$children%
    example/atomic_three/hes_sparsity.cpp
%$$
$head Examples$$
The file $cref atomic_three_hes_sparsity.cpp$$ contains an example and test
that uses this routine.
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic/three_hes_sparsity.hpp
Third generation atomic Hessian dependency and sparsity patterns.
*/
/*!
atomic_three to Hessian dependency and sparsity calculations.

\param parameter_x [in]
contains the values for arguments that are parameters.

\param select_x [in]
which domain components to include in the dependency or sparsity pattern.

\param select_y [in]
which range components to include in the dependency or sparsity pattern.

\param pattern_out [out]
is the sparsity pattern for lower triangle of Hessian.
*/
// BEGIN_PROTOTYPE
template <class Base>
bool atomic_three<Base>::hes_sparsity(
    const vector<Base>&                     parameter_x  ,
    const vector<bool>&                     select_x     ,
    const vector<bool>&                     select_y     ,
    sparse_rc< vector<size_t> >&            pattern_out  )
// END_PROTOTYPE
{   return false; }
/*!
Link from for_hes_sweep to atomic_three.

\tparam InternalSparsity
Is the used internaly for sparsity calculations; i.e.,
sparse_pack or sparse_list.

\param parameter_x
is parameter arguments to the function, other components are nan.

\param x_index
is the variable index, on the tape, for the arguments to this function.
This size of x_index is n, the number of arguments to this function.
The index zero is used for parameters.

\param y_index
is the variable index, on the tape, for the results for this function.
This size of y_index is m, the number of results for this function.
The index zero is used for parameters.

\param for_jac_sparsity
On input, for j = 0, ... , n-1, the sparsity pattern with index x_index[j],
is the forward Jacobian sparsity for the j-th argument to this atomic function.

\param rev_jac_sparsity
On input, for i = 0, ... , m-1, the sparsity pattern with index y_index[i],
is the reverse Jacobian sparsity for the i-th result to this atomic function.
This shows which components of the result affect the function we are
computing the Hessian of.

\param var_sparsity
This is the sparsity pattern for the Hessian. On input, the non-linear
terms in the atomic fuction have not been included. Upon return, they
have been included.
*/
template <class Base>
template <class InternalSparsity>
bool atomic_three<Base>::for_hes_sparsity(
    const vector<Base>&              parameter_x      ,
    const local::pod_vector<size_t>& x_index          ,
    const local::pod_vector<size_t>& y_index          ,
    const InternalSparsity&          for_jac_sparsity ,
    const InternalSparsity&          rev_jac_sparsity ,
    InternalSparsity&                var_sparsity     )
{   typedef typename InternalSparsity::const_iterator const_iterator;
    CPPAD_ASSERT_UNKNOWN( rev_jac_sparsity.end() == 1 );
    size_t n      = x_index.size();
    size_t m      = y_index.size();
    //
    // select_x
    vector<bool> select_x(n);
    for(size_t j = 0; j < n; j++)
    {   // check if should compute pattern w.r.t x[j]
        const_iterator itr(for_jac_sparsity, x_index[j]);
        size_t i = *itr;
        select_x[j] = i < for_jac_sparsity.end();
        CPPAD_ASSERT_UNKNOWN( x_index[j] > 0 || ! select_x[j] );
    }
    //
    // bool select_y
    vector<bool> select_y(m);
    for(size_t i = 0; i < m; i++)
    {   // check if we should include y[i]
        select_y[i] = rev_jac_sparsity.is_element(y_index[i], 0);
        CPPAD_ASSERT_UNKNOWN( y_index[i] > 0 || ! select_y[i] );
    }
    //
    // call user's version of atomic function
    sparse_rc< vector<size_t> > pattern_out;
    bool ok = hes_sparsity(parameter_x, select_x, select_y, pattern_out);
    if( ! ok )
        return ok;
    //
    // add new elements to Hessian sparisty in calling routine
    const vector<size_t>& row( pattern_out.row() );
    const vector<size_t>& col( pattern_out.col() );
    for(size_t k = 0; k < pattern_out.nnz(); ++k)
    {   size_t r = row[k];
        size_t c = col[k];
# ifndef NDEBUG
        if( c > r )
        {   std::string msg = afun_name();
            msg += ": this atomic function's hes_sparsity "
                "pattern_out is not lower traingular";
            CPPAD_ASSERT_KNOWN(false, msg.c_str() );
        }
# endif
        const_iterator itr_1(for_jac_sparsity, x_index[r]);
        size_t v1 = *itr_1;
        while( v1 < for_jac_sparsity.end() )
        {   var_sparsity.binary_union(
                v1, v1, x_index[r], for_jac_sparsity
             );
             v1 = *(++itr_1);
        }
        if( c != r )
        {   const_iterator itr_2(for_jac_sparsity, x_index[c]);
            size_t v2 = *itr_2;
            while( v2 < for_jac_sparsity.end() )
            {   var_sparsity.binary_union(
                    v2, v2, x_index[c], for_jac_sparsity
                );
                v2 = *(++itr_2);
            }
        }
    }
    return ok;
}

} // END_CPPAD_NAMESPACE

# endif