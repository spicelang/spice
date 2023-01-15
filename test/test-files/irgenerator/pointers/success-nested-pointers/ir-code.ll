; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [7 x i8] c"1: %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [7 x i8] c"2: %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [7 x i8] c"3: %d\0A\00", align 1
@printf.str.3 = private unnamed_addr constant [7 x i8] c"4: %d\0A\00", align 1
@anon.array.0 = private unnamed_addr constant [4 x i32] [i32 1, i32 2, i32 3, i32 4]
@printf.str.4 = private unnamed_addr constant [7 x i8] c"1: %d\0A\00", align 1

define private void @_p__void__void__testProc__intarrayptrptrptr(ptr %0) {
  %nums = alloca ptr, align 8
  %nums1 = alloca ptr, align 8
  %nums2 = alloca ptr, align 8
  store ptr %0, ptr %nums, align 8
  %2 = load ptr, ptr %nums, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = load ptr, ptr %3, align 8
  store ptr %4, ptr %nums1, align 8
  %5 = load ptr, ptr %nums1, align 8
  %6 = load ptr, ptr %5, align 8
  store ptr %6, ptr %nums2, align 8
  %7 = load ptr, ptr %nums2, align 8
  %8 = getelementptr inbounds i32, ptr %7, i32 2
  store i32 10, ptr %8, align 4
  %9 = load ptr, ptr %nums2, align 8
  %10 = getelementptr inbounds i32, ptr %9, i32 0
  %11 = load i32, ptr %10, align 4
  %12 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %11)
  %13 = load ptr, ptr %nums2, align 8
  %14 = getelementptr inbounds i32, ptr %13, i32 1
  %15 = load i32, ptr %14, align 4
  %16 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %15)
  %17 = load ptr, ptr %nums2, align 8
  %18 = getelementptr inbounds i32, ptr %17, i32 2
  %19 = load i32, ptr %18, align 4
  %20 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 %19)
  %21 = load ptr, ptr %nums2, align 8
  %22 = getelementptr inbounds i32, ptr %21, i32 3
  %23 = load i32, ptr %22, align 4
  %24 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i32 %23)
  ret void
}

declare i32 @printf(ptr noundef, ...)

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %1 = alloca [4 x i32], align 4
  %intArray = alloca [4 x i32], align 4
  %intArray1 = alloca ptr, align 8
  %intArray2 = alloca ptr, align 8
  %intArray3 = alloca ptr, align 8
  %2 = alloca ptr, align 8
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %1, ptr @anon.array.0, i64 16, i1 false)
  store [4 x i32] [i32 1, i32 2, i32 3, i32 4], ptr %intArray, align 4
  %5 = getelementptr inbounds [4 x i32], ptr %intArray, i32 0, i32 1
  %6 = load i32, ptr %5, align 4
  %7 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i32 %6)
  store ptr %intArray, ptr %intArray1, align 8
  store ptr %intArray1, ptr %intArray2, align 8
  store ptr %intArray2, ptr %intArray3, align 8
  %8 = load ptr, ptr %intArray3, align 8
  %9 = load ptr, ptr %8, align 8
  %10 = load ptr, ptr %9, align 8
  %11 = getelementptr inbounds [4 x i32], ptr %10, i32 0, i32 0
  store ptr %11, ptr %2, align 8
  store ptr %2, ptr %3, align 8
  store ptr %3, ptr %4, align 8
  call void @_p__void__void__testProc__intarrayptrptrptr(ptr %4)
  %12 = load i32, ptr %result, align 4
  ret i32 %12
}

; Function Attrs: argmemonly nocallback nofree nounwind willreturn
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { argmemonly nocallback nofree nounwind willreturn }
