; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__TreeNode__TreeNodeptr_int = type { ptr, i32 }

@anon.struct.0 = private unnamed_addr constant %__TreeNode__TreeNodeptr_int zeroinitializer
@printf.str.0 = private unnamed_addr constant [21 x i8] c"Root node number: %d\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %rootNode = alloca %__TreeNode__TreeNodeptr_int, align 8
  %childNode1 = alloca %__TreeNode__TreeNodeptr_int, align 8
  %childNode2 = alloca %__TreeNode__TreeNodeptr_int, align 8
  %childNode21 = alloca %__TreeNode__TreeNodeptr_int, align 8
  %curNode = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %rootNode, ptr @anon.struct.0, i64 16, i1 false)
  %1 = getelementptr inbounds %__TreeNode__TreeNodeptr_int, ptr %childNode1, i32 0, i32 0
  store ptr %rootNode, ptr %1, align 8
  %2 = getelementptr inbounds %__TreeNode__TreeNodeptr_int, ptr %childNode1, i32 0, i32 1
  store i32 1, ptr %2, align 4
  %3 = getelementptr inbounds %__TreeNode__TreeNodeptr_int, ptr %childNode2, i32 0, i32 0
  store ptr %rootNode, ptr %3, align 8
  %4 = getelementptr inbounds %__TreeNode__TreeNodeptr_int, ptr %childNode2, i32 0, i32 1
  store i32 2, ptr %4, align 4
  %5 = getelementptr inbounds %__TreeNode__TreeNodeptr_int, ptr %childNode21, i32 0, i32 0
  store ptr %childNode2, ptr %5, align 8
  %6 = getelementptr inbounds %__TreeNode__TreeNodeptr_int, ptr %childNode21, i32 0, i32 1
  store i32 3, ptr %6, align 4
  store ptr %childNode21, ptr %curNode, align 8
  br label %while.head.L21

while.head.L21:                                   ; preds = %while.body.L21, %0
  %7 = load ptr, ptr %curNode, align 8
  %8 = getelementptr inbounds %__TreeNode__TreeNodeptr_int, ptr %7, i32 0, i32 0
  %9 = load ptr, ptr %8, align 8
  %10 = icmp ne ptr %9, null
  br i1 %10, label %while.body.L21, label %while.exit.L21

while.body.L21:                                   ; preds = %while.head.L21
  %11 = load ptr, ptr %curNode, align 8
  %12 = getelementptr inbounds %__TreeNode__TreeNodeptr_int, ptr %11, i32 0, i32 0
  %13 = load ptr, ptr %12, align 8
  store ptr %13, ptr %curNode, align 8
  br label %while.head.L21

while.exit.L21:                                   ; preds = %while.head.L21
  %14 = load ptr, ptr %curNode, align 8
  %15 = getelementptr inbounds %__TreeNode__TreeNodeptr_int, ptr %14, i32 0, i32 1
  %16 = load i32, ptr %15, align 4
  %17 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %16)
  %18 = load i32, ptr %result, align 4
  ret i32 %18
}

; Function Attrs: argmemonly nocallback nofree nounwind willreturn
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

declare i32 @printf(ptr noundef, ...)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { argmemonly nocallback nofree nounwind willreturn }
