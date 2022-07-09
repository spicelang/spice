; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__TreeNode__TreeNodeptr_int = type { %_s__TreeNode__TreeNodeptr_int*, i32 }

@0 = private unnamed_addr constant [21 x i8] c"Root node number: %d\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %rootNode = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %0 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %rootNode1 = alloca %_s__TreeNode__TreeNodeptr_int*, align 8
  %childNode1 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %1 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %2 = alloca %_s__TreeNode__TreeNodeptr_int*, align 8
  %childNode2 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %3 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %4 = alloca %_s__TreeNode__TreeNodeptr_int*, align 8
  %childNode21 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %5 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %6 = alloca %_s__TreeNode__TreeNodeptr_int*, align 8
  %curNode = alloca %_s__TreeNode__TreeNodeptr_int*, align 8
  %7 = alloca %_s__TreeNode__TreeNodeptr_int*, align 8
  %curNode2 = alloca %_s__TreeNode__TreeNodeptr_int*, align 8
  %8 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store %_s__TreeNode__TreeNodeptr_int* null, %_s__TreeNode__TreeNodeptr_int** %rootNode1, align 8
  %9 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %rootNode1, align 8
  %10 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %0, i32 0, i32 0
  store %_s__TreeNode__TreeNodeptr_int* %9, %_s__TreeNode__TreeNodeptr_int** %10, align 8
  %11 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %0, i32 0, i32 1
  store i32 0, i32* %11, align 4
  store %_s__TreeNode__TreeNodeptr_int* %0, %_s__TreeNode__TreeNodeptr_int** %2, align 8
  %12 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %2, align 8
  %13 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %1, i32 0, i32 0
  store %_s__TreeNode__TreeNodeptr_int* %12, %_s__TreeNode__TreeNodeptr_int** %13, align 8
  %14 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %1, i32 0, i32 1
  store i32 1, i32* %14, align 4
  store %_s__TreeNode__TreeNodeptr_int* %0, %_s__TreeNode__TreeNodeptr_int** %4, align 8
  %15 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %4, align 8
  %16 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %3, i32 0, i32 0
  store %_s__TreeNode__TreeNodeptr_int* %15, %_s__TreeNode__TreeNodeptr_int** %16, align 8
  %17 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %3, i32 0, i32 1
  store i32 2, i32* %17, align 4
  store %_s__TreeNode__TreeNodeptr_int* %3, %_s__TreeNode__TreeNodeptr_int** %6, align 8
  %18 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %6, align 8
  %19 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %5, i32 0, i32 0
  store %_s__TreeNode__TreeNodeptr_int* %18, %_s__TreeNode__TreeNodeptr_int** %19, align 8
  %20 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %5, i32 0, i32 1
  store i32 3, i32* %20, align 4
  store %_s__TreeNode__TreeNodeptr_int* %5, %_s__TreeNode__TreeNodeptr_int** %7, align 8
  br label %while.cond

while.cond:                                       ; preds = %while, %entry
  %21 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %7, align 8
  %22 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %7, align 8
  %23 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %22, i32 0, i32 0
  %24 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %23, align 8
  store %_s__TreeNode__TreeNodeptr_int* null, %_s__TreeNode__TreeNodeptr_int** %curNode2, align 8
  %25 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %curNode2, align 8
  %26 = icmp ne %_s__TreeNode__TreeNodeptr_int* %24, %25
  store i1 %26, i1* %8, align 1
  %27 = load i1, i1* %8, align 1
  br i1 %27, label %while, label %while.end

while:                                            ; preds = %while.cond
  %28 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %7, align 8
  %29 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %7, align 8
  %30 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %29, i32 0, i32 0
  %31 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %30, align 8
  store %_s__TreeNode__TreeNodeptr_int* %31, %_s__TreeNode__TreeNodeptr_int** %7, align 8
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %32 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %7, align 8
  %33 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %7, align 8
  %34 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %33, i32 0, i32 1
  %35 = load i32, i32* %34, align 4
  %36 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @0, i32 0, i32 0), i32 %35)
  %37 = load i32, i32* %result, align 4
  ret i32 %37
}

declare i32 @printf(i8*, ...)
