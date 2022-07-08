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
  %1 = alloca %_s__TreeNode__TreeNodeptr_int*, align 8
  %childNode1 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %2 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %3 = alloca %_s__TreeNode__TreeNodeptr_int*, align 8
  %childNode2 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %4 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %5 = alloca %_s__TreeNode__TreeNodeptr_int*, align 8
  %childNode21 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %6 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %7 = alloca %_s__TreeNode__TreeNodeptr_int*, align 8
  %curNode = alloca %_s__TreeNode__TreeNodeptr_int*, align 8
  %8 = alloca %_s__TreeNode__TreeNodeptr_int*, align 8
  %9 = alloca %_s__TreeNode__TreeNodeptr_int*, align 8
  %10 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store %_s__TreeNode__TreeNodeptr_int* null, %_s__TreeNode__TreeNodeptr_int** %1, align 8
  %11 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %1, align 8
  %12 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %0, i32 0, i32 0
  store %_s__TreeNode__TreeNodeptr_int* %11, %_s__TreeNode__TreeNodeptr_int** %12, align 8
  %13 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %0, i32 0, i32 1
  store i32 0, i32* %13, align 4
  %14 = load %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %0, align 8
  store %_s__TreeNode__TreeNodeptr_int %14, %_s__TreeNode__TreeNodeptr_int* %rootNode, align 8
  %15 = load %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %rootNode, align 8
  store %_s__TreeNode__TreeNodeptr_int* %rootNode, %_s__TreeNode__TreeNodeptr_int** %3, align 8
  store %_s__TreeNode__TreeNodeptr_int* %rootNode, %_s__TreeNode__TreeNodeptr_int** %3, align 8
  %16 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %3, align 8
  %17 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %2, i32 0, i32 0
  store %_s__TreeNode__TreeNodeptr_int* %16, %_s__TreeNode__TreeNodeptr_int** %17, align 8
  %18 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %2, i32 0, i32 1
  store i32 1, i32* %18, align 4
  %19 = load %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %2, align 8
  store %_s__TreeNode__TreeNodeptr_int %19, %_s__TreeNode__TreeNodeptr_int* %childNode1, align 8
  %20 = load %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %rootNode, align 8
  store %_s__TreeNode__TreeNodeptr_int* %rootNode, %_s__TreeNode__TreeNodeptr_int** %5, align 8
  store %_s__TreeNode__TreeNodeptr_int* %rootNode, %_s__TreeNode__TreeNodeptr_int** %5, align 8
  %21 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %5, align 8
  %22 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %4, i32 0, i32 0
  store %_s__TreeNode__TreeNodeptr_int* %21, %_s__TreeNode__TreeNodeptr_int** %22, align 8
  %23 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %4, i32 0, i32 1
  store i32 2, i32* %23, align 4
  %24 = load %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %4, align 8
  store %_s__TreeNode__TreeNodeptr_int %24, %_s__TreeNode__TreeNodeptr_int* %childNode2, align 8
  %25 = load %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %childNode2, align 8
  store %_s__TreeNode__TreeNodeptr_int* %childNode2, %_s__TreeNode__TreeNodeptr_int** %7, align 8
  store %_s__TreeNode__TreeNodeptr_int* %childNode2, %_s__TreeNode__TreeNodeptr_int** %7, align 8
  %26 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %7, align 8
  %27 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %6, i32 0, i32 0
  store %_s__TreeNode__TreeNodeptr_int* %26, %_s__TreeNode__TreeNodeptr_int** %27, align 8
  %28 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %6, i32 0, i32 1
  store i32 3, i32* %28, align 4
  %29 = load %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %6, align 8
  store %_s__TreeNode__TreeNodeptr_int %29, %_s__TreeNode__TreeNodeptr_int* %childNode21, align 8
  %30 = load %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %childNode21, align 8
  store %_s__TreeNode__TreeNodeptr_int* %childNode21, %_s__TreeNode__TreeNodeptr_int** %8, align 8
  store %_s__TreeNode__TreeNodeptr_int* %childNode21, %_s__TreeNode__TreeNodeptr_int** %8, align 8
  %31 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %8, align 8
  store %_s__TreeNode__TreeNodeptr_int* %31, %_s__TreeNode__TreeNodeptr_int** %curNode, align 8
  br label %while.cond

while.cond:                                       ; preds = %while, %entry
  %32 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %curNode, align 8
  %33 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %curNode, align 8
  %34 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %32, i32 0, i32 0
  %35 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %34, align 8
  store %_s__TreeNode__TreeNodeptr_int* null, %_s__TreeNode__TreeNodeptr_int** %9, align 8
  %36 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %9, align 8
  %37 = icmp ne %_s__TreeNode__TreeNodeptr_int* %35, %36
  store i1 %37, i1* %10, align 1
  %38 = load i1, i1* %10, align 1
  br i1 %38, label %while, label %while.end

while:                                            ; preds = %while.cond
  %39 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %curNode, align 8
  %40 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %curNode, align 8
  %41 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %39, i32 0, i32 0
  %42 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %41, align 8
  %43 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %curNode, align 8
  store %_s__TreeNode__TreeNodeptr_int* %42, %_s__TreeNode__TreeNodeptr_int** %curNode, align 8
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %44 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %curNode, align 8
  %45 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %curNode, align 8
  %46 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %44, i32 0, i32 1
  %47 = load i32, i32* %46, align 4
  %48 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @0, i32 0, i32 0), i32 %47)
  %49 = load i32, i32* %result, align 4
  ret i32 %49
}

declare i32 @printf(i8*, ...)
