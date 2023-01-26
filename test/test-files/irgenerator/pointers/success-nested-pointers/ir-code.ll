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
  %nums2 = alloca [4 x i32], align 4
  store ptr %0, ptr %nums, align 8
  %2 = load ptr, ptr %nums, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = load ptr, ptr %3, align 8
  store ptr %4, ptr %nums1, align 8
  %5 = load ptr, ptr %nums1, align 8
  %6 = load [4 x i32], ptr %5, align 4
  store [4 x i32] %6, ptr %nums2, align 4
  %7 = getelementptr inbounds [4 x i32], ptr %nums2, i32 0, i32 2
  store i32 10, ptr %7, align 4
  %8 = getelementptr inbounds [4 x i32], ptr %nums2, i32 0, i32 0
  %9 = load i32, ptr %8, align 4
  %10 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %9)
  %11 = getelementptr inbounds [4 x i32], ptr %nums2, i32 0, i32 1
  %12 = load i32, ptr %11, align 4
  %13 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %12)
  %14 = getelementptr inbounds [4 x i32], ptr %nums2, i32 0, i32 2
  %15 = load i32, ptr %14, align 4
  %16 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 %15)
  %17 = getelementptr inbounds [4 x i32], ptr %nums2, i32 0, i32 3
  %18 = load i32, ptr %17, align 4
  %19 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i32 %18)
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
  store i32 0, ptr %result, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %1, ptr @anon.array.0, i64 16, i1 false)
  store [4 x i32] [i32 1, i32 2, i32 3, i32 4], ptr %intArray, align 4
  %2 = getelementptr inbounds [4 x i32], ptr %intArray, i32 0, i32 1
  %3 = load i32, ptr %2, align 4
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i32 %3)
  store ptr %intArray, ptr %intArray1, align 8
  store ptr %intArray1, ptr %intArray2, align 8
  call void @_p__void__void__testProc__intarrayptrptrptr(ptr %intArray2)
  %5 = load i32, ptr %result, align 4
  ret i32 %5
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
