; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__TreeNode__TreeNodeptr_int = type { %_s__TreeNode__TreeNodeptr_int*, i32 }

@0 = private unnamed_addr constant [21 x i8] c"Root node number: %d\00", align 1

define i32 @main() {
entry.l13:
  %result = alloca i32, align 4
  %0 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %rootNode = alloca %_s__TreeNode__TreeNodeptr_int*, align 8
  %1 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %2 = alloca %_s__TreeNode__TreeNodeptr_int*, align 8
  %3 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %4 = alloca %_s__TreeNode__TreeNodeptr_int*, align 8
  %5 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %6 = alloca %_s__TreeNode__TreeNodeptr_int*, align 8
  %7 = alloca %_s__TreeNode__TreeNodeptr_int*, align 8
  %8 = alloca %_s__TreeNode__TreeNodeptr_int*, align 8
  %9 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store %_s__TreeNode__TreeNodeptr_int* null, %_s__TreeNode__TreeNodeptr_int** %rootNode, align 8
  %10 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %rootNode, align 8
  %11 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %0, i32 0, i32 0
  store %_s__TreeNode__TreeNodeptr_int* %10, %_s__TreeNode__TreeNodeptr_int** %11, align 8
  %12 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %0, i32 0, i32 1
  store i32 0, i32* %12, align 4
  store %_s__TreeNode__TreeNodeptr_int* %0, %_s__TreeNode__TreeNodeptr_int** %2, align 8
  %13 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %2, align 8
  %14 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %1, i32 0, i32 0
  store %_s__TreeNode__TreeNodeptr_int* %13, %_s__TreeNode__TreeNodeptr_int** %14, align 8
  %15 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %1, i32 0, i32 1
  store i32 1, i32* %15, align 4
  store %_s__TreeNode__TreeNodeptr_int* %0, %_s__TreeNode__TreeNodeptr_int** %4, align 8
  %16 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %4, align 8
  %17 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %3, i32 0, i32 0
  store %_s__TreeNode__TreeNodeptr_int* %16, %_s__TreeNode__TreeNodeptr_int** %17, align 8
  %18 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %3, i32 0, i32 1
  store i32 2, i32* %18, align 4
  store %_s__TreeNode__TreeNodeptr_int* %3, %_s__TreeNode__TreeNodeptr_int** %6, align 8
  %19 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %6, align 8
  %20 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %5, i32 0, i32 0
  store %_s__TreeNode__TreeNodeptr_int* %19, %_s__TreeNode__TreeNodeptr_int** %20, align 8
  %21 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %5, i32 0, i32 1
  store i32 3, i32* %21, align 4
  store %_s__TreeNode__TreeNodeptr_int* %5, %_s__TreeNode__TreeNodeptr_int** %7, align 8
  br label %while.cond.l21

while.cond.l21:                                   ; preds = %while.l21, %entry.l13
  %22 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %7, align 8
  %23 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %7, align 8
  %24 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %23, i32 0, i32 0
  %25 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %24, align 8
  store %_s__TreeNode__TreeNodeptr_int* null, %_s__TreeNode__TreeNodeptr_int** %8, align 8
  %26 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %8, align 8
  %27 = icmp ne %_s__TreeNode__TreeNodeptr_int* %25, %26
  store i1 %27, i1* %9, align 1
  %28 = load i1, i1* %9, align 1
  br i1 %28, label %while.l21, label %while.end.l21

while.l21:                                        ; preds = %while.cond.l21
  %29 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %7, align 8
  %30 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %7, align 8
  %31 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %30, i32 0, i32 0
  %32 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %31, align 8
  store %_s__TreeNode__TreeNodeptr_int* %32, %_s__TreeNode__TreeNodeptr_int** %7, align 8
  br label %while.cond.l21

while.end.l21:                                    ; preds = %while.cond.l21
  %33 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %7, align 8
  %34 = load %_s__TreeNode__TreeNodeptr_int*, %_s__TreeNode__TreeNodeptr_int** %7, align 8
  %35 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, %_s__TreeNode__TreeNodeptr_int* %34, i32 0, i32 1
  %36 = load i32, i32* %35, align 4
  %37 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @0, i32 0, i32 0), i32 %36)
  %38 = load i32, i32* %result, align 4
  ret i32 %38
}

declare i32 @printf(i8*, ...)
