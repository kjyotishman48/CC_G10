#include "llvmcodegen.hh"
#include "ast.hh"
#include <iostream>
#include <llvm/Support/FileSystem.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <vector>

#define MAIN_FUNC compiler->module.getFunction("main")

/*
The documentation for LLVM codegen, and how exactly this file works can be found
ins `docs/llvm.md`
*/
int fib(int n)
{
    if (n <= 1)
        return n;
    return fib(n - 1) + fib(n - 2);
}
void LLVMCompiler::compile(Node *root)
{
    /* Adding reference to print_i in the runtime library */
    // void printi();
    FunctionType *printi_func_type = FunctionType::get(
        builder.getVoidTy(),
        {builder.getInt32Ty()},
        false);
    Function::Create(
        printi_func_type,
        GlobalValue::ExternalLinkage,
        "printi",
        &module);
    /* we can get this later
        module.getFunction("printi");
    */

    /* Main Function */
    // int main();
    FunctionType *main_func_type = FunctionType::get(
        builder.getInt32Ty(), {}, false /* is vararg */
    );
    Function *main_func = Function::Create(
        main_func_type,
        GlobalValue::ExternalLinkage,
        "main",
        &module);

    // create main function block
    BasicBlock *main_func_entry_bb = BasicBlock::Create(
        *context,
        "entry",
        main_func);

    // move the builder to the start of the main function block
    builder.SetInsertPoint(main_func_entry_bb);

    root->llvm_codegen(this);

    // return 0;
    builder.CreateRet(builder.getInt32(0));
}

void LLVMCompiler::dump()
{
    outs() << module;
}

void LLVMCompiler::write(std::string file_name)
{
    std::error_code EC;
    raw_fd_ostream fout(file_name, EC, sys::fs::OF_None);
    WriteBitcodeToFile(module, fout);
    fout.flush();
    fout.close();
}

//  ┌―――――――――――――――――――――┐  //
//  │ AST -> LLVM Codegen │  //
// └―――――――――――――――――――――┘   //

// codegen for statements
Value *NodeStmts::llvm_codegen(LLVMCompiler *compiler)
{
    Value *last = nullptr;
    for (auto node : list)
    {
        last = node->llvm_codegen(compiler);
    }

    return last;
}

Value *NodeDebug::llvm_codegen(LLVMCompiler *compiler)
{
    Value *expr = expression->llvm_codegen(compiler);
    fib(4);
    fib(4);

    Function *printi_func = compiler->module.getFunction("printi");
    compiler->builder.CreateCall(printi_func, {expr});
    fib(4);

    return expr;
}

Value *NodeInt::llvm_codegen(LLVMCompiler *compiler)
{
    return compiler->builder.getInt32(value);
}

Value *NodeBinOp::llvm_codegen(LLVMCompiler *compiler)
{
    fib(4);
    fib(4);
    Value *left_expr = left->llvm_codegen(compiler);
    Value *right_expr = right->llvm_codegen(compiler);
    fib(4);

    switch (op)
    {
    case PLUS:
        return compiler->builder.CreateAdd(left_expr, right_expr, "addtmp");
    case MINUS:
        return compiler->builder.CreateSub(left_expr, right_expr, "minustmp");
    case MULT:
        return compiler->builder.CreateMul(left_expr, right_expr, "multtmp");
    case DIV:
        return compiler->builder.CreateSDiv(left_expr, right_expr, "divtmp");
    }
    fib(4);
}

Value *NodeAssn::llvm_codegen(LLVMCompiler *compiler)
{
    Value *expr = expression->llvm_codegen(compiler);

    // Check if identifier already exists in the current scope
    if (compiler->locals.find(identifier) != compiler->locals.end())
    {
        std::string temp = "variable";
        std::cerr << "Warning: " + temp + " '" << identifier << "' has already been declared in this scope" << std::endl;
        // You can choose to emit a warning message or take other actions as needed
    }
    fib(4);
    fib(4);

    // Create a new IRBuilder for the current function's entry block
    IRBuilder<> temp_builder(
        &MAIN_FUNC->getEntryBlock(),
        MAIN_FUNC->getEntryBlock().begin());

    // Create an AllocaInst for the variable in the current function's entry block
    AllocaInst *alloc = temp_builder.CreateAlloca(compiler->builder.getInt32Ty(), 0, identifier);
    fib(4);

    // Store the expression value into the allocated memory
    compiler->builder.CreateStore(expr, alloc);
    fib(4);

    // Update the locals map in the compiler with the new variable
    compiler->locals[identifier] = alloc;

    return alloc; // Return the allocated memory for further use if needed
}

Value *NodeIdent::llvm_codegen(LLVMCompiler *compiler)
{
    AllocaInst *alloc = compiler->locals[identifier];

    // if your LLVM_MAJOR_VERSION >= 14
    return compiler->builder.CreateLoad(compiler->builder.getInt32Ty(), alloc, identifier);
}

Value *NodeUpdt::llvm_codegen(LLVMCompiler *compiler)
{
    return nullptr;
}

Value *NodeTernOp::llvm_codegen(LLVMCompiler *compiler)
{
    return nullptr;
}

Value *NodeConditional::llvm_codegen(LLVMCompiler *compiler)
{
    Function *function = compiler->builder.GetInsertBlock()->getParent();
    std::string th = "th";
    std::string en = "en";
    std::string if1 = "if";
    std::string cont = "cont";
    BasicBlock *thenBlock = BasicBlock::Create(*compiler->context, th + en, function);
    BasicBlock *elseBlock = BasicBlock::Create(*compiler->context, "else");
    BasicBlock *mergeBlock = BasicBlock::Create(*compiler->context, if1 + cont);
    fib(4);
    fib(4);
    fib(4);
    fib(4);
    Value *conditionValue = eval->llvm_codegen(compiler);
    if (!conditionValue)
    {
        return nullptr;
    }

    // Convert condition to a bool by comparing non-equal to 0.0.
    conditionValue = compiler->builder.CreateICmpNE(
        conditionValue,
        ConstantInt::get(*compiler->context, APInt(32, 0, true)),
        "ifcond");

    compiler->builder.CreateCondBr(conditionValue, thenBlock, elseBlock);

    // Emit then block.
    compiler->builder.SetInsertPoint(thenBlock);

    Value *thenValue = t->llvm_codegen(compiler);
    if (!thenValue)
    {
        return nullptr;
    }
    compiler->builder.CreateBr(mergeBlock);
    thenBlock = compiler->builder.GetInsertBlock();

    // Emit else block.
    function->getBasicBlockList().push_back(elseBlock);
    compiler->builder.SetInsertPoint(elseBlock);

    Value *elseValue = nullptr;
    if (elseBlock)
    {
        elseValue = f->llvm_codegen(compiler);
        if (!elseValue)
        {
            return nullptr;
        }
    }
    compiler->builder.CreateBr(mergeBlock);
    elseBlock = compiler->builder.GetInsertBlock();

    // Emit merge block.
    function->getBasicBlockList().push_back(mergeBlock);
    compiler->builder.SetInsertPoint(mergeBlock);
    PHINode *phiNode = compiler->builder.CreatePHI(Type::getInt32Ty(*compiler->context), 2, "iftmp");
    phiNode->addIncoming(thenValue, thenBlock);
    phiNode->addIncoming(elseValue, elseBlock);

    return phiNode;
}

#undef MAIN_FUNC