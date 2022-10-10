; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [7 x i8] c"1: %d\0A\00", align 1
@1 = private unnamed_addr constant [7 x i8] c"2: %d\0A\00", align 1
@2 = private unnamed_addr constant [7 x i8] c"3: %d\0A\00", align 1
@3 = private unnamed_addr constant [7 x i8] c"4: %d\0A\00", align 1
@intArray = global [4 x i32] [i32 1, i32 2, i32 3, i32 4]
@4 = private unnamed_addr constant [7 x i8] c"1: %d\0A\00", align 1

define internal void @_p__void__testProc__intarrayptrptrptr(ptr %0) {
entry.l1:
  %nums = alloca ptr, align 8
  %nums1 = alloca ptr, align 8
  %nums2 = alloca ptr, align 8
  store ptr %0, ptr %nums, align 8
  %1 = load ptr, ptr %nums, align 8
  %2 = load ptr, ptr %1, align 8
  %3 = load ptr, ptr %2, align 8
  store ptr %3, ptr %2, align 8
  %4 = load ptr, ptr %2, align 8
  store ptr %4, ptr %nums1, align 8
  %5 = load ptr, ptr %nums1, align 8
  %6 = load ptr, ptr %5, align 8
  store ptr %6, ptr %5, align 8
  %7 = load ptr, ptr %5, align 8
  store ptr %7, ptr %nums2, align 8
  %8 = load ptr, ptr %nums2, align 8
  %9 = getelementptr inbounds i32, ptr %8, i32 2
  store i32 10, ptr %9, align 4
  %10 = load ptr, ptr %nums2, align 8
  %11 = getelementptr inbounds i32, ptr %10, i32 0
  %12 = load i32, ptr %11, align 4
  %13 = call i32 (ptr, ...) @printf(ptr @0, i32 %12)
  %14 = load ptr, ptr %nums2, align 8
  %15 = getelementptr inbounds i32, ptr %14, i32 1
  %16 = load i32, ptr %15, align 4
  %17 = call i32 (ptr, ...) @printf(ptr @1, i32 %16)
  %18 = load ptr, ptr %nums2, align 8
  %19 = getelementptr inbounds i32, ptr %18, i32 2
  %20 = load i32, ptr %19, align 4
  %21 = call i32 (ptr, ...) @printf(ptr @2, i32 %20)
  %22 = load ptr, ptr %nums2, align 8
  %23 = getelementptr inbounds i32, ptr %22, i32 3
  %24 = load i32, ptr %23, align 4
  %25 = call i32 (ptr, ...) @printf(ptr @3, i32 %24)
  ret void
}

declare i32 @printf(ptr, ...)

define i32 @main() {
entry.l11:
  %result = alloca i32, align 4
  %intArray = alloca [4 x i32], align 4
  %0 = alloca ptr, align 8
  %1 = alloca ptr, align 8
  %2 = alloca ptr, align 8
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca i1, align 1
  store i32 0, ptr %result, align 4
  %7 = load [4 x i32], ptr @intArray, align 4
  store [4 x i32] %7, ptr %intArray, align 4
  %8 = getelementptr inbounds [4 x i32], ptr %intArray, i32 0, i32 1
  %9 = load i32, ptr %8, align 4
  %10 = call i32 (ptr, ...) @printf(ptr @4, i32 %9)
  store ptr %intArray, ptr %0, align 8
  store ptr %0, ptr %1, align 8
  store ptr %1, ptr %2, align 8
  %11 = load ptr, ptr %2, align 8
  %12 = load ptr, ptr %11, align 8
  %13 = load ptr, ptr %12, align 8
  %14 = getelementptr inbounds [4 x i32], ptr %13, i32 0, i32 0
  store ptr %14, ptr %3, align 8
  store ptr %3, ptr %4, align 8
  store ptr %4, ptr %5, align 8
  call void @_p__void__testProc__intarrayptrptrptr(ptr %5)
  store i1 true, ptr %6, align 1
  %15 = load i32, ptr %result, align 4
  ret i32 %15
}
