# [QR Decomposition](https://en.wikipedia.org/wiki/QR_decomposition)
## [Gram-Schmit orthogonalization](https://en.wikipedia.org/wiki/Gram%E2%80%93Schmidt#Numerical_stability)
1. The Gram-Schmidt process is inherently numerically unstable;
2. It is the ease of implementation, which makes this a useful algorithm to use for prototyping if a pre-built linear algebra library is unavailable, which is almost never the case;
## [Householder reflection](https://en.wikipedia.org/wiki/Householder_reflection)
1. It is greater numerical stability than the Gram–Schmidt method;
2. The complexity of the algorithm is $O(n^3)$;
3. Householder transformation is inherently the most simple of the numerically stable QR decomposition algorithms;
4. However, the Householder reflection algorithm is bandwidth heavy and not parallelizable,  
   as every reflection that produces a new zero element changes the entirety of both Q and R matrices;
## [Givens rotation](https://en.wikipedia.org/wiki/Givens_rotation)
1. In practice, Givens rotations are not actually performed by building a whole matrix and doing a matrix multiplication;
2. Instead, it does the equivalent of the sparse Givens matrix multiplication, without the extra work of handling the sparse elements;
3. The Givens rotation procedure is useful in situations where only a relatively few off diagonal elements need to be zeroed;
4. It is more easily parallelized than Householder transformations;
5. Givens rotations is the most involved to implement, as the ordering of the rows required to fully exploit the algorithm is not trivial to determine;
6. A significant advantage in that each new zero element $a_{ij}$ affects only the row with the element to be zeroed (i) and a row above (j);
7. The Givens rotation algorithm is more bandwidth efficient and parallelizable than the Householder reflection technique;
## Column pivoting
Pivoted QR differs from ordinary Gram-Schmidt in that it takes the largest remaining column at the beginning of each new step column pivoting and thus introduces a **permutation matrix P**:
$$
AP = QR \iff A = QRP^T
$$
Column pivoting is useful when A is (nearly) [rank deficient](https://en.wikipedia.org/wiki/Rank_deficient), or is suspected of being so; it can also improve numerical accuracy.
P is usually chosen so that the diagonal elements of R are non-increasing: $|r_{11}|\geq |r_{22}|\geq \cdots \geq |r_{nn}|$.  
This can be used to find the (numerical) rank of A at lower computational cost than a [singular value decomposition](https://en.wikipedia.org/wiki/Singular_value_decomposition), forming the basis of so-called [rank-revealing QR algorithms](https://en.wikipedia.org/wiki/Rank-revealing_QR_algorithm).
## Using for solution to linear inverse problems
1. Compared to the direct matrix inverse, inverse solutions using QR decomposition are more numerically stable as evidenced by their reduced [condition numbers](https://en.wikipedia.org/wiki/Condition_number);
2. To solve the underdetermined (m < n) linear problem $Ax = b$ where the matrix A has dimensions $m \times n$ and **rank m**,  first find the QR factorization of the transpose of A: $A^T=QR$, where Q is an orthogonal matrix (i.e. $Q^T=Q^{-1}$), and R has a special form:
$
R={\begin{bmatrix}R_{1}\\0\end{bmatrix}}
$
Here $R_{1}$ is a square $m \times m$ right triangular matrix, and the zero matrix has dimension $(n-m) \times m$. After some algebra, it can be shown that a solution to the inverse problem can be expressed as: 
$ 
\mathbf {x} = Q \begin{bmatrix}(R_{1}^{T})^{-1} \mathbf {b} \\ 0 \end{bmatrix}
$
where one may either find $R_1^{-1}$ by Gaussian elimination or compute $(R_{1}^T)^{-1} \mathbf {b}$ directly by forward substitution. **The latter technique enjoys greater numerical accuracy and lower computations**;  

1. To find a solution ${\hat {\mathbf {x} }}$ to the overdetermined ($m \geq n$) problem, $Ax = b$  which minimizes the norm $||A{\hat {\mathbf {x} }}-\mathbf {b}||$, first find the QR factorization of the transpose of A: $A=QR$. The solution can then be expressed as
$
{\hat {\mathbf {x} }}=R_{1}^{-1} (Q_{1}^{\textsf {T}}\mathbf {b})
$, where $Q_{1}$ is an $m \times n$ matrix containing the first n columns of the full orthonormal basis Q and where $R_{1}$ is as before. Equivalent to the underdetermined case, back substitution can be used to quickly and accurately find this $\hat {\mathbf {x} }$ without explicitly inverting $R_{1}$. ($Q_{1}$ and $R_{1}$ are often provided by numerical libraries as an "economic" QR decomposition.)