; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__TreeNode__TreeNodeptr_int = type { ptr, i32 }

@0 = private unnamed_addr constant [21 x i8] c"Root node number: %d\00", align 1

define i32 @main() {
entry.l13:
  %result = alloca i32, align 4
  %0 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %rootNode = alloca ptr, align 8
  %1 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %2 = alloca ptr, align 8
  %3 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %4 = alloca ptr, align 8
  %5 = alloca %_s__TreeNode__TreeNodeptr_int, align 8
  %6 = alloca ptr, align 8
  %7 = alloca ptr, align 8
  %8 = alloca ptr, align 8
  %9 = alloca i1, align 1
  store i32 0, ptr %result, align 4
  store ptr null, ptr %rootNode, align 8
  %10 = load ptr, ptr %rootNode, align 8
  %11 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, ptr %0, i32 0, i32 0
  store ptr %10, ptr %11, align 8
  %12 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, ptr %0, i32 0, i32 1
  store i32 0, ptr %12, align 4
  store ptr %0, ptr %2, align 8
  %13 = load ptr, ptr %2, align 8
  %14 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, ptr %1, i32 0, i32 0
  store ptr %13, ptr %14, align 8
  %15 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, ptr %1, i32 0, i32 1
  store i32 1, ptr %15, align 4
  store ptr %0, ptr %4, align 8
  %16 = load ptr, ptr %4, align 8
  %17 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, ptr %3, i32 0, i32 0
  store ptr %16, ptr %17, align 8
  %18 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, ptr %3, i32 0, i32 1
  store i32 2, ptr %18, align 4
  store ptr %3, ptr %6, align 8
  %19 = load ptr, ptr %6, align 8
  %20 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, ptr %5, i32 0, i32 0
  store ptr %19, ptr %20, align 8
  %21 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, ptr %5, i32 0, i32 1
  store i32 3, ptr %21, align 4
  store ptr %5, ptr %7, align 8
  br label %while.cond.l21

while.cond.l21:                                   ; preds = %while.l21, %entry.l13
  %22 = load ptr, ptr %7, align 8
  %23 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, ptr %22, i32 0, i32 0
  %24 = load ptr, ptr %23, align 8
  store ptr null, ptr %8, align 8
  %25 = load ptr, ptr %8, align 8
  %26 = icmp ne ptr %24, %25
  store i1 %26, ptr %9, align 1
  %27 = load i1, ptr %9, align 1
  br i1 %27, label %while.l21, label %while.end.l21

while.l21:                                        ; preds = %while.cond.l21
  %28 = load ptr, ptr %7, align 8
  %29 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, ptr %28, i32 0, i32 0
  %30 = load ptr, ptr %29, align 8
  store ptr %30, ptr %7, align 8
  br label %while.cond.l21

while.end.l21:                                    ; preds = %while.cond.l21
  %31 = load ptr, ptr %7, align 8
  %32 = getelementptr inbounds %_s__TreeNode__TreeNodeptr_int, ptr %31, i32 0, i32 1
  %33 = load i32, ptr %32, align 4
  %34 = call i32 (ptr, ...) @printf(ptr @0, i32 %33)
  %35 = load i32, ptr %result, align 4
  ret i32 %35
}

declare i32 @printf(ptr, ...)
