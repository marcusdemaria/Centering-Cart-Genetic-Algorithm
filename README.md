# Preface and Project Description
Solving the popular genetic algorithm problem of getting a cart to optimally center itself at a specific point. MECHTRON 2MD3 Winter 2024 Takehome Midterm Project.

We began with a directory that contains a near-complete implementation of a Genetic Programming experiment to solve the Cart Centering (isotropic rocket) problem.  The gp.cpp program will evolve trees for 100 generations and print statistics on the best tree at each generation. It was also expected that we needed to implement a few extra functions to the directory on our own, on top of understanding and testing analytics on the code that was already given. <br><br>

Finalized results with data collection and answers to any questions to be added to PDF file titled gp.pdf.

# Function Implementation
void LinkedBinaryTree::printExpression(Node* v), <br>
This function must recursively print the expression tree with parentheses. For example, a tree representing postfix expression “1 2 + 3 *” is printed as “((1+2)*3)”

LinkedBinaryTree createRandExpressionTree(int max_depth, mt19937& rng), <br>
This function should create and return a randomly generated expression tree with a depth no greater than max_depth. (The function currently simply returns the expression tree “a + b” where a and b are the X position and velocity of the cart, respectively.) 

void LinkedBinaryTree::deleteSubtreeMutator(mt19937& rng), <br>
This function should delete a randomly selected subtree from the tree.

void LinkedBinaryTree::addSubtreeMutator(mt19937& rng), <br>
This function should add a randomly created subtree to the tree.

Implement a comparator ADT called LexLessThan which performs a lexicographic comparison of two trees TA and TB as follows: If the scores of TA and TB differ by less than 0.01, then TA is “less than” TB if TA has more nodes than TB. Otherwise, compare the trees by their score. The goal of using this comparator is to favour simpler trees only when their scores are similar. Repeat the experiment from part 1 using the new comparator. To do so, you must uncomment the line under “sort using comparator class”. After doing so, does evolution produce simpler trees? Create a line plot comparing the size of the best tree during evolution with and without the LexLessThan comparator<br>

Implement a crossover operator in which you sample two parents and create two children by swapping random subtrees from each parent. Note there is no placeholder code for crossover in the prexisting code from the directory. You must determine where and how to add this feature. When results are ready, add a line to your plot labelled “rand init + crossover + mutation” to show the results of this experiment. <br>

# Data Graphing and Explanation
The following finalized code with implemented functions from the section above was to be ran, and results were to be plotted and graphed for each time a new function was added, including the required plots for the LexLessThan and crossover implementations. You should see a significant improvement in the best fitness after implementing createRandExpressionTree, and a further improvement after implementing the mutation operators. <br>
Example terminal output with all functions added: <br>
Best tree: <br>
(((a > a) + (((b > b) * abs(((abs((b / abs(a))) * (((a / b) / a) / b)) > ((abs((a * (b / b))) - (abs(a) / <br>
a)) + a)))) * a)) - (b + a)) <br>
Generation: 99 <br>
Size: 42 <br>
Depth: 11 <br>
Fitness: -0.0889936 <br> <br>

All plots, graphs, and other figures can be seen on the PDF results sheet - gp.pdf.

# Short Answer Questions
In order to get a better understanding of the results, along with genetic programming, there were a few short answer questions to answer to check understanding. These questions relate to tree traversal as the root of the code deals with trees in various forms such as binary trees, and other operations and vocabulary related to them. <br><br>
The answers can be seen on the PDF results sheet - gp.pdf <br><br>
What kind of tree traversal (e.g. InOrder, PostOrder, etc.) is used by each function:
1. LinkedBinaryTree::printExpression() 
2. LinkedBinaryTree::evaluateExpression(const Position& p, double a, double b)
3. LinkedBinaryTree::depth()
4. LinkedBinaryTree::size() <br> <br>
The End.
