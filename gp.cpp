#include <math.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <random>
#include <stack>
#include <vector>

#include "cartCentering.h"

using namespace std;

/*
Parts of functions were created with help from AI such as chatgpt as well as collaboration with peers in the class
*/

// return a double uniformly sampled in (0,1)
double randDouble(mt19937& rng) {
  return std::uniform_real_distribution<>{0, 1}(rng);
}
// return uniformly sampled 0 or 1
bool randChoice(mt19937& rng) {
  return std::uniform_int_distribution<>{0, 1}(rng);
}
// return a random integer uniformly sampled in (min, max)
int randInt(mt19937& rng, const int& min, const int& max) {
  return std::uniform_int_distribution<>{min, max}(rng);
}

// return true if op is a suported operation, otherwise return false
bool isOp(string op) {
  if (op == "+")
    return true;
  else if (op == "-")
    return true;
  else if (op == "*")
    return true;
  else if (op == "/")
    return true;
  else if (op == ">")
    return true;
  else if (op == "abs")
    return true;
  else
    return false;
}

int arity(string op) {
  if (op == "abs")
    return 1;
  else
    return 2;
}

typedef string Elem;

class LinkedBinaryTree {
 public:
  struct Node {
    Elem elt;
    string name;
    Node* par;
    Node* left;
    Node* right;
    Node() : elt(), par(NULL), name(""), left(NULL), right(NULL) {}
    int depth() {
      if (par == NULL) return 0;
      return par->depth() + 1;
    }
  };

  class Position {
   private:
    Node* v;

   public:
    Position(Node* _v = NULL) : v(_v) {}
    Node* getNode() const { return v; } // Accessor method to get the Node pointer for crossover
    Elem& operator*() { return v->elt; }
    Position left() const { return Position(v->left); }
    void setLeft(Node* n) { v->left = n; }
    Position right() const { return Position(v->right); }
    void setRight(Node* n) { v->right = n; }
    Position parent() const  // get parent
    {
      return Position(v->par);
    }
    bool isRoot() const  // root of the tree?
    {
      return v->par == NULL;
    }
    bool isExternal() const  // an external node?
    {
      return v->left == NULL && v->right == NULL;
    }
    friend class LinkedBinaryTree;  // give tree access
  };
  typedef vector<Position> PositionList;

 public:
  LinkedBinaryTree() : _root(NULL), score(0), steps(0), generation(0) {}

  // copy constructor
  LinkedBinaryTree(const LinkedBinaryTree& t) {
    _root = copyPreOrder(t.root());
    score = t.getScore();
    steps = t.getSteps();
    generation = t.getGeneration();
  }

  // copy assignment operator
  LinkedBinaryTree& operator=(const LinkedBinaryTree& t) {
    if (this != &t) {
      // if tree already contains data, delete it
      if (_root != NULL) {
        PositionList pl = positions();
        for (auto& p : pl) delete p.v;
      }
      _root = copyPreOrder(t.root());
      score = t.getScore();
      steps = t.getSteps();
      generation = t.getGeneration();
    }
    return *this;
  }

  // destructor
  ~LinkedBinaryTree() {
    if (_root != NULL) {
      PositionList pl = positions();
      for (auto& p : pl) delete p.v;
    }
  }

  int size() const { return size(_root); }
  int size(Node* root) const;
  int depth() const;
  bool empty() const { return size() == 0; };
  Node* root() const { return _root; }
  PositionList positions() const;
  void addRoot() { _root = new Node; }
  void addRoot(Elem e) {
    _root = new Node;
    _root->elt = e;
  }
  void nameRoot(string name) { _root->name = name; }
  void addLeftChild(const Position& p, const Node* n);
  void addLeftChild(const Position& p);
  void addRightChild(const Position& p, const Node* n);
  void addRightChild(const Position& p);
  void printExpression() { printExpression(_root); }
  void printExpression(Node* v);
  double evaluateExpression(double a, double b) {
    return evaluateExpression(Position(_root), a, b);
  };
  double evaluateExpression(const Position& p, double a, double b);
  long getGeneration() const { return generation; }
  void setGeneration(int g) { generation = g; }
  double getScore() const { return score; }
  void setScore(double s) { score = s; }
  double getSteps() const { return steps; }
  void setSteps(double s) { steps = s; }
  void randomExpressionTree(Node* p, const int& maxDepth, mt19937& rng);
  void randomExpressionTree(const int& maxDepth, mt19937& rng) {
    randomExpressionTree(_root, maxDepth, rng);
  }
  void deleteSubtreeMutator(mt19937& rng);
  void deleteSubtree(Node* subtreeRoot); // Declaration of deleteSubtree helper function
  void addSubtreeMutator(mt19937& rng, const int maxDepth);
  void crossover(const LinkedBinaryTree& parent1, const LinkedBinaryTree& parent2, mt19937& rng);
  
 protected:                                        // local utilities
  void preorder(Node* v, PositionList& pl) const;  // preorder utility
  Node* copyPreOrder(const Node* root);
  double score;     // mean reward over 20 episodes
  double steps;     // mean steps-per-episode over 20 episodes
  long generation;  // which generation was tree "born"
 private:
  Node* _root;  // pointer to the root
};


class LexLessThan {
public:
    bool operator()(const LinkedBinaryTree& TA, const LinkedBinaryTree& TB) const {
        // Calculate the difference in scores
        double scoreDifference = TA.getScore() - TB.getScore();

        // If the scores differ by less than 0.01, favor the tree with more nodes
        if (abs(scoreDifference) < 0.01) {
            // Compare by the number of nodes
            return TA.size() > TB.size();
        } else {
            // Otherwise, compare by scores
            return TA.getScore() < TB.getScore();
        }
    }
};

// add the tree rooted at node child as this tree's left child
void LinkedBinaryTree::addLeftChild(const Position& p, const Node* child) {
  Node* v = p.v;
  v->left = copyPreOrder(child);  // deep copy child
  v->left->par = v;
}

// add the tree rooted at node child as this tree's right child
void LinkedBinaryTree::addRightChild(const Position& p, const Node* child) {
  Node* v = p.v;
  v->right = copyPreOrder(child);  // deep copy child
  v->right->par = v;
}

void LinkedBinaryTree::addLeftChild(const Position& p) {
  Node* v = p.v;
  v->left = new Node;
  v->left->par = v;
}

void LinkedBinaryTree::addRightChild(const Position& p) {
  Node* v = p.v;
  v->right = new Node;
  v->right->par = v;
}

// return a list of all nodes
LinkedBinaryTree::PositionList LinkedBinaryTree::positions() const {
  PositionList pl;
  preorder(_root, pl);
  return PositionList(pl);
}

void LinkedBinaryTree::preorder(Node* v, PositionList& pl) const {
  pl.push_back(Position(v));
  if (v->left != NULL) preorder(v->left, pl);
  if (v->right != NULL) preorder(v->right, pl);
}

int LinkedBinaryTree::size(Node* v) const {
  int lsize = 0;
  int rsize = 0;
  if (v->left != NULL) lsize = size(v->left);
  if (v->right != NULL) rsize = size(v->right);
  return 1 + lsize + rsize;
}

int LinkedBinaryTree::depth() const {
  PositionList pl = positions();
  int depth = 0;
  for (auto& p : pl) depth = std::max(depth, p.v->depth());
  return depth;
}

LinkedBinaryTree::Node* LinkedBinaryTree::copyPreOrder(const Node* root) {
  if (root == NULL) return NULL;
  Node* nn = new Node;
  nn->elt = root->elt;
  nn->left = copyPreOrder(root->left);
  if (nn->left != NULL) nn->left->par = nn;
  nn->right = copyPreOrder(root->right);
  if (nn->right != NULL) nn->right->par = nn;
  return nn;
}

void LinkedBinaryTree::printExpression(Node* v) {
    if (v != nullptr) {
        // If the node represents an operator with arity greater than 1 ex: +, -, *, /, >, print with parentheses
        if (isOp(v->elt) && arity(v->elt) > 1) {
            cout << "(";
            printExpression(v->left);
            cout << " " << v->elt << " ";
            printExpression(v->right);
            cout << ")";
        } 
        // If the node represents a unary operator ex: abs , print with parentheses around the operand
        else if (isOp(v->elt)) {
            cout << v->elt;
            cout << "(";
            printExpression(v->left);
            cout << ")";
        } 
        // If the node represents an operand, print the operand
        else {
            cout << (v->elt);
        }
    } 
}
  
double evalOp(string op, double x, double y = 0) {
  double result;
  if (op == "+")
    result = x + y;
  else if (op == "-")
    result = x - y;
  else if (op == "*")
    result = x * y;
  else if (op == "/") {
    result = x / y;
  } else if (op == ">") {
    result = x > y ? 1 : -1;
  } else if (op == "abs") {
    result = abs(x);
  } else
    result = 0;
  return isnan(result) || !isfinite(result) ? 0 : result;
}

double LinkedBinaryTree::evaluateExpression(const Position& p, double a,
                                            double b) {
  if (!p.isExternal()) {
    auto x = evaluateExpression(p.left(), a, b);
    if (arity(p.v->elt) > 1) {
      auto y = evaluateExpression(p.right(), a, b);
      return evalOp(p.v->elt, x, y);
    } else {
      return evalOp(p.v->elt, x);
    }
  } else {
    if (p.v->elt == "a")
      return a;
    else if (p.v->elt == "b")
      return b;
    else
      return stod(p.v->elt);
  }
}

// declaring initializion of any functions needed inside of the mutator functions
LinkedBinaryTree createRandExpressionTree(int max_depth, mt19937& rng);


void LinkedBinaryTree::deleteSubtree(Node* node) {
  if (!node) return;

  // Recursively delete left and right subtrees
  deleteSubtree(node->left);
  node->left =
      nullptr;  // Set the pointer to nullptr to avoid dangling pointers
  deleteSubtree(node->right);
  node->right =
      nullptr;  // Set the pointer to nullptr to avoid dangling pointers

  // Delete the node itself
  delete node;
}


void LinkedBinaryTree::deleteSubtreeMutator(mt19937& rng) {
    if (!_root || (_root->left == nullptr && _root->right == nullptr)) return; // Tree is empty or just has a root

    PositionList allPositions = positions();

    if (allPositions.size() <= 1) return; // Only root is present

    int randomIndex = randInt(rng, 1, allPositions.size() - 1); // Avoid selecting the root
    Position positionToDelete = allPositions[randomIndex];

    Node* nodeToDelete = positionToDelete.v;
    if (!positionToDelete.isRoot()) {
        // Delete all children of the selected node
        deleteSubtree(nodeToDelete->left);
        nodeToDelete->left = nullptr;
        deleteSubtree(nodeToDelete->right);
        nodeToDelete->right = nullptr;

        // Replace the node's content with "a" or "b" randomly
        nodeToDelete->elt = randChoice(rng) ? "a" : "b";
    }
    
}


void LinkedBinaryTree::addSubtreeMutator(mt19937& rng, const int maxDepth) {
    // Generate a random expression tree
    LinkedBinaryTree randomTree = createRandExpressionTree(maxDepth, rng);

    // Check if the generated tree is not empty
    if (randomTree.root() != nullptr) {
        // If the current tree is empty, set its root to the random tree generated
        if (_root == nullptr) {
            _root = copyPreOrder(randomTree.root());
        } else {
            // Randomly decide whether to add the random tree to the left or right side
            if (randChoice(rng)) {
                addLeftChild(Position(_root), randomTree.root());
            } else {
                addRightChild(Position(_root), randomTree.root());
            }
        }
    }
    
}

bool operator<(const LinkedBinaryTree& x, const LinkedBinaryTree& y) {
  return x.getScore() < y.getScore();
}

LinkedBinaryTree createExpressionTree(string postfix) {
  stack<LinkedBinaryTree> tree_stack;
  stringstream ss(postfix);
  // Split each line into words
  string token;
  while (getline(ss, token, ' ')) {
    LinkedBinaryTree t;
    if (!isOp(token)) {
      t.addRoot(token);
      tree_stack.push(t);
    } else {
      t.addRoot(token);
      if (arity(token) > 1) {
        LinkedBinaryTree r = tree_stack.top();
        tree_stack.pop();
        t.addRightChild(t.root(), r.root());
      }
      LinkedBinaryTree l = tree_stack.top();
      tree_stack.pop();
      t.addLeftChild(t.root(), l.root());
      tree_stack.push(t);
    }
  }
  return tree_stack.top();
}

LinkedBinaryTree createRandExpressionTree(int max_depth, mt19937& rng) {
    // Available operands for the expression tree
    vector<string> operands = {"a", "b"};

    // Base case: if maximum depth is reached, randomly select an operand and return a leaf node
    if (max_depth == 0) {
        LinkedBinaryTree leaf;
        // Select a random operand
        int operandIndex = uniform_int_distribution<>(0, operands.size() - 1)(rng);
        leaf.addRoot(operands[operandIndex]);
        return leaf;
    }

    // Recursive case: Create a tree with an operator as the root and subtrees as operands
    LinkedBinaryTree tree;
    // Available operators for the expression tree
    vector<string> operators = {"+", "-", "*", "/", "abs", ">"};
    // Select a random operator
    int operatorIndex = uniform_int_distribution<>(0, operators.size() - 1)(rng);
    tree.addRoot(operators[operatorIndex]);

    // Randomly decide whether to continue growing the tree or add a leaf node
    bool growLeft = randChoice(rng);
    bool growRight = randChoice(rng);

    if (growLeft) {
        // Recursively create a random expression tree and attach it as the left child
        LinkedBinaryTree leftSubtree = createRandExpressionTree(max_depth - 1, rng);
        tree.addLeftChild(tree.root(), leftSubtree.root());
    } else {
        // Add a leaf node as the left child
        LinkedBinaryTree leftLeaf;
        // Select a random operand
        int operandIndex = uniform_int_distribution<>(0, operands.size() - 1)(rng);
        leftLeaf.addRoot(operands[operandIndex]);
        tree.addLeftChild(tree.root(), leftLeaf.root());
    }

    if (growRight) {
        // Recursively create a random expression tree and attach it as the right child
        LinkedBinaryTree rightSubtree = createRandExpressionTree(max_depth - 1, rng);
        tree.addRightChild(tree.root(), rightSubtree.root());
    } else {
        // Add a leaf node as the right child
        LinkedBinaryTree rightLeaf;
        // Select a random operand
        int operandIndex = uniform_int_distribution<>(0, operands.size() - 1)(rng);
        rightLeaf.addRoot(operands[operandIndex]);
        tree.addRightChild(tree.root(), rightLeaf.root());
    }

    return tree;
}

void LinkedBinaryTree::crossover(const LinkedBinaryTree& parent1, const LinkedBinaryTree& parent2, mt19937& rng) {
  PositionList pl1 = parent1.positions();// Select random positions in each parent tree
  PositionList pl2 = parent2.positions();
  int randIndex1 = randInt(rng, 0, pl1.size() - 1);
  int randIndex2 = randInt(rng, 0, pl2.size() - 1);

  Node* subtree1 = pl1[randIndex1].v; // Extract nodes from the selected positions
  Node* subtree2 = pl2[randIndex2].v;

  Node* tempLeft = subtree1->left; // Swap subtrees between parent trees to create child trees
  Node* tempRight = subtree1->right;
  subtree1->left = subtree2->left;
  subtree1->right = subtree2->right;
  subtree2->left = tempLeft;
  subtree2->right = tempRight;
}

// evaluate tree t in the cart centering task
void evaluate(mt19937& rng, LinkedBinaryTree& t, const int& num_episode,
              bool animate) {
  cartCentering env;
  double mean_score = 0.0;
  double mean_steps = 0.0;
  for (int i = 0; i < num_episode; i++) {
    double episode_score = 0.0;
    int episode_steps = 0;
    env.reset(rng);
    while (!env.terminal()) {
      int action = t.evaluateExpression(env.getCartXPos(), env.getCartXVel());
      episode_score += env.update(action, animate);
      episode_steps++;
    }
    mean_score += episode_score;
    mean_steps += episode_steps;
  }
  t.setScore(mean_score / num_episode);
  t.setSteps(mean_steps / num_episode);
}

int main() {
  mt19937 rng(42);
  // Experiment parameters
  const int NUM_TREE = 50;
  const int MAX_DEPTH_INITIAL = 1;
  const int MAX_DEPTH = 20;
  const int NUM_EPISODE = 20;
  const int MAX_GENERATIONS = 100;

  // Create an initial "population" of expression trees
  vector<LinkedBinaryTree> trees;
  for (int i = 0; i < NUM_TREE; i++) {
    LinkedBinaryTree t = createRandExpressionTree(MAX_DEPTH_INITIAL, rng);
    trees.push_back(t);
  }

  // Genetic Algorithm loop
  LinkedBinaryTree best_tree;
  std::cout << "generation,fitness,steps,size,depth" << std::endl;
  for (int g = 1; g <= MAX_GENERATIONS; g++) {

    // Fitness evaluation
    for (auto& t : trees) {
      if (t.getGeneration() < g - 1) continue;  // skip if not new
      evaluate(rng, t, NUM_EPISODE, false);
    }

    // sort trees using overloaded "<" op (worst->best)
    std::sort(trees.begin(), trees.end());

    // // sort trees using comparaor class (worst->best)
    std::sort(trees.begin(), trees.end(), LexLessThan());

    // erase worst 50% of trees (first half of vector)
    trees.erase(trees.begin(), trees.begin() + NUM_TREE / 2);

    // Print stats for best tree
    best_tree = trees[trees.size() - 1];
    std::cout << g << ",";
    std::cout << best_tree.getScore() << ",";
    std::cout << best_tree.getSteps() << ",";
    std::cout << best_tree.size() << ",";
    std::cout << best_tree.depth() << std::endl;

    while (trees.size() < NUM_TREE) {
      // Select two random parent trees from survivors
      LinkedBinaryTree parent1 = trees[randInt(rng, 0, (NUM_TREE / 2) - 1)];
      LinkedBinaryTree parent2 = trees[randInt(rng, 0, (NUM_TREE / 2) - 1)];

      // Create two child trees using crossover operator
      LinkedBinaryTree child1(parent1);
      LinkedBinaryTree child2(parent2);
      child1.setGeneration(g);
      child2.setGeneration(g);
      child1.crossover(parent1, parent2, rng);
      child2.crossover(parent1, parent2, rng);

      // Perform mutation (delete subtree) on child1
      child1.deleteSubtreeMutator(rng);
      // Perform mutation (add subtree) on child2
      child2.addSubtreeMutator(rng, MAX_DEPTH);

      // Add child trees to the population
      trees.push_back(child1);
      trees.push_back(child2);
  }
    
    
  }

  // // Evaluate best tree with animation
  // const int num_episode = 3;
  // evaluate(rng, best_tree, num_episode, true);

  // Print best tree info
  std::cout << std::endl << "Best tree:" << std::endl;
  best_tree.printExpression();
  std::cout << endl;
  std::cout << "Generation: " << best_tree.getGeneration() << endl;
  std::cout << "Size: " << best_tree.size() << std::endl;
  std::cout << "Depth: " << best_tree.depth() << std::endl;
  std::cout << "Fitness: " << best_tree.getScore() << std::endl << std::endl;
}