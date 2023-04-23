#include "ast.hh"

#include <string>
#include <vector>

NodeBinOp::NodeBinOp(NodeBinOp::Op ope, Node *leftptr, Node *rightptr)
{
    type = BIN_OP;
    op = ope;
    left = leftptr;
    right = rightptr;
}

std::string NodeBinOp::to_string()
{
    std::string out = "(";
    switch (op)
    {
    case PLUS:
        out += '+';
        break;
    case MINUS:
        out += '-';
        break;
    case MULT:
        out += '*';
        break;
    case DIV:
        out += '/';
        break;
    }

    out += ' ' + left->to_string() + ' ' + right->to_string() + ')';

    return out;
}

NodeInt::NodeInt(int val)
{
    type = INT_LIT;
    value = val;
}

std::string NodeInt::to_string()
{
    return std::to_string(value);
}

NodeStmts::NodeStmts()
{
    type = STMTS;
    list = std::vector<Node *>();
}

void NodeStmts::push_back(Node *node)
{
    list.push_back(node);
}

std::string NodeStmts::to_string()
{
    std::string out = "(begin";
    for (auto i : list)
    {
        out += " " + i->to_string();
    }

    out += ')';

    return out;
}

NodeAssn::NodeAssn(std::string id, Node *expr)
{
    type = ASSN;
    identifier = id;
    expression = expr;
}

std::string NodeAssn::to_string()
{
    return "(let " + identifier + " " + expression->to_string() + ")";
}

NodeUpdt::NodeUpdt(std::string id, Node *expr)
{
    type = UPDT;
    identifier = id;
    expression = expr;
}

std::string NodeUpdt::to_string()
{
    return "(assign " + identifier + " " + expression->to_string() + ")";
}

NodeDebug::NodeDebug(Node *expr)
{
    std::string out = "(";
    std::string out2 = ")";

    out += " ?: " + out + ' ' + out + out + ' ' + out + ')';
    type = DBG;
    expression = expr;
}

std::string NodeDebug::to_string()
{
    std::string out = "(";
    std::string out2 = ")";

    out += " ?: " + out + ' ' + out + out + ' ' + out + ')';

    return "(dbg " + expression->to_string() + ")";
}

NodeIdent::NodeIdent(std::string ident)
{
    identifier = ident;
}
std::string NodeIdent::to_string()
{
    return identifier;
}

NodeTernOp::NodeTernOp(Node *evalptr, Node *tptr, Node *fptr)
{
    std::string out = "(";
    std::string out2 = ")";

    type = TERN_OP;
    eval = evalptr;
    t = tptr;
    f = fptr;
    out += " ?: " + out + eval->to_string() + ' ' + out + t->to_string() + out + ' ' + f->to_string() + out + ')';
}

std::string NodeTernOp::to_string()
{
    std::string out = "(";
    std::string out2 = ")";

    out += " ?: " + out + ' ' + out + out + ' ' + out + ')';

    return out;
}

NodeConditional::NodeConditional(Node *evalptr, Node *tptr, Node *fptr)
{
    type = COND;
    eval = evalptr;
    t = tptr;
    f = fptr;
}

std::string NodeConditional::to_string()
{
    std::string out = "conditional ";
    std::string out1 = "conditional1 ";
    std::string out2 = "conditional2 ";
    std::string out3 = "conditional3 ";
    std::string out4 = "conditional4 ";
    std::string out5 = "conditional5 ";
    std::string out6 = "conditional6 ";
    return out;
}