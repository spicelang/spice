; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%_s__TreeNode__TreeNodeptr_int = type { ptr, i32 }

@0 = private unnamed_addr constant [21 x i8] c"Root node number: %d\00", align 1

define i32 @main() {
entry.l13:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %0 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %rootNode = alloca ptr, align 8
  store ptr null, ptr %rootNode, align 8
  %1 = load ptr, ptr %rootNode, align 8
  %2 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, ptr %0, i32 0, i32 0
  store ptr %1, ptr %2, align 8
  %3 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, ptr %0, i32 0, i32 1
  store i32 0, ptr %3, align 4
  %4 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %5 = alloca ptr, align 8
  store ptr %0, ptr %5, align 8
  %6 = load ptr, ptr %5, align 8
  %7 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, ptr %4, i32 0, i32 0
  store ptr %6, ptr %7, align 8
  %8 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, ptr %4, i32 0, i32 1
  store i32 1, ptr %8, align 4
  %9 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %10 = alloca ptr, align 8
  store ptr %0, ptr %10, align 8
  %11 = load ptr, ptr %10, align 8
  %12 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, ptr %9, i32 0, i32 0
  store ptr %11, ptr %12, align 8
  %13 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, ptr %9, i32 0, i32 1
  store i32 2, ptr %13, align 4
  %14 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %15 = alloca ptr, align 8
  store ptr %9, ptr %15, align 8
  %16 = load ptr, ptr %15, align 8
  %17 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, ptr %14, i32 0, i32 0
  store ptr %16, ptr %17, align 8
  %18 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, ptr %14, i32 0, i32 1
  store i32 3, ptr %18, align 4
  %19 = alloca ptr, align 8
  store ptr %14, ptr %19, align 8
  br label %while.cond.l21

while.cond.l21:                                   ; preds = %while.l21, %entry.l13
  %20 = load ptr, ptr %19, align 8
  %21 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, ptr %20, i32 0, i32 0
  %22 = load ptr, ptr %21, align 8
  %23 = alloca ptr, align 8
  store ptr null, ptr %23, align 8
  %24 = load ptr, ptr %23, align 8
  %25 = icmp ne ptr %22, %24
  %26 = alloca i1, align 1
  store i1 %25, ptr %26, align 1
  %27 = load i1, ptr %26, align 1
  br i1 %27, label %while.l21, label %while.end.l21

while.l21:                                        ; preds = %while.cond.l21
  %28 = load ptr, ptr %19, align 8
  %29 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, ptr %28, i32 0, i32 0
  %30 = load ptr, ptr %29, align 8
  store ptr %30, ptr %19, align 8
  br label %while.cond.l21

while.end.l21:                                    ; preds = %while.cond.l21
  %31 = load ptr, ptr %19, align 8
  %32 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, ptr %31, i32 0, i32 1
  %33 = load i32, ptr %32, align 4
  %34 = call i32 (ptr, ...) @printf(ptr @0, i32 %33)
  %35 = load i32, ptr %result, align 4
  ret i32 %35
}

declare i32 @printf(ptr, ...)
