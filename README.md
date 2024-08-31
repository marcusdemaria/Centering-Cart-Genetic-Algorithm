# Preface and Project Description
Solving the popular genetic algorithm problem of getting a cart to optimally center itself at a specific point. MECHTRON 2MD3 Winter 2024 Takehome Midterm Project.

We began with a directory that contains a near-complete implementation of a Genetic Programming experiment to solve the Cart Centering (isotropic rocket) problem.  The gp.cpp program will evolve trees for 100 generations and print statistics on the best tree at each generation. It was also expected that we needed to implement a few extra functions to the directory on our own, on top of understanding and testing analytics on the code that was already given.

# Function Implementation
void LinkedBinaryTree::printExpression(Node* v), <br>
This function must recursively print the expression tree with parentheses. For example, a tree representing postfix expression “1 2 + 3 *” is printed as “((1+2)*3)”

LinkedBinaryTree createRandExpressionTree(int max_depth, mt19937& rng), <br>
This function should create and return a randomly generated expression tree with a depth no greater than max_depth. (The function currently simply returns the expression tree “a + b” where a and b are the X position and velocity of the cart, respectively.) 

void LinkedBinaryTree::deleteSubtreeMutator(mt19937& rng), <br>
This function should delete a randomly selected subtree from the tree.

void LinkedBinaryTree::addSubtreeMutator(mt19937& rng), <br>
This function should add a randomly created subtree to the tree.

# Data Graphing and Explanation
The following results were to be plotted and graphed for each time a new function was added. You should see a significant improvement in the best fitness after implementing createRandExpressionTree, and a further improvement after implementing the mutation operators. <br>
Example terminal output with all functions added: <br>
Best tree: <br>
(((a > a) + (((b > b) * abs(((abs((b / abs(a))) * (((a / b) / a) / b)) > ((abs((a * (b / b))) - (abs(a) / <br>
a)) + a)))) * a)) - (b + a)) <br>
Generation: 99 <br>
Size: 42 <br>
Depth: 11 <br>
Fitness: -0.0889936 <br>

# Short Answer Questions
What kind of tree traversal (e.g. InOrder, PostOrder, etc.) is used by each function:
1. LinkedBinaryTree::printExpression() 
2. LinkedBinaryTree::evaluateExpression(const Position& p, double a, double b)
3. LinkedBinaryTree::depth()
4. LinkedBinaryTree::size() <br> <br>
The End.
