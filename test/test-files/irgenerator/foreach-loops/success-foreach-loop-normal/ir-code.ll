; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__int__ArrayIterator__intptr_long_long = type { ptr, i64, i64 }

@anon.array.0 = private unnamed_addr constant [7 x i32] [i32 1, i32 5, i32 4, i32 0, i32 12, i32 12345, i32 9]
@printf.str.0 = private unnamed_addr constant [10 x i8] c"Item: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %1 = alloca [7 x i32], align 4
  %intArray = alloca [7 x i32], align 4
  %2 = alloca %__int__ArrayIterator__intptr_long_long, align 8
  %3 = alloca %__int__ArrayIterator__intptr_long_long, align 8
  %item = alloca i32, align 4
  store i32 0, ptr %result, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %1, ptr @anon.array.0, i64 28, i1 false)
  store [7 x i32] [i32 1, i32 5, i32 4, i32 0, i32 12, i32 12345, i32 9], ptr %intArray, align 4
  %4 = getelementptr inbounds [7 x i32], ptr %intArray, i32 0, i32 0
  %5 = call %__int__ArrayIterator__intptr_long_long @"_f__void__std/iterator/array-iterator::ArrayIterator<int>__iterate__intarray_long"(ptr %4, i64 7)
  store %__int__ArrayIterator__intptr_long_long %5, ptr %3, align 8
  br label %foreach.head.L5

foreach.head.L5:                                  ; preds = %foreach.tail.L5, %0
  %6 = call i1 @_mf__ArrayIterator_int__bool__isValid(ptr %3)
  br i1 %6, label %foreach.body.L5, label %foreach.exit.L5

foreach.body.L5:                                  ; preds = %foreach.head.L5
  %7 = call ptr @_mf__ArrayIterator_int__intref__get(ptr %3)
  %8 = load i32, ptr %7, align 4
  store i32 %8, ptr %item, align 4
  %9 = load i32, ptr %item, align 4
  %10 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %9)
  br label %foreach.tail.L5

foreach.tail.L5:                                  ; preds = %foreach.body.L5
  call void @_mp__ArrayIterator_int__void__next(ptr %3)
  br label %foreach.head.L5

foreach.exit.L5:                                  ; preds = %foreach.head.L5
  %11 = load i32, ptr %result, align 4
  ret i32 %11
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

declare %__int__ArrayIterator__intptr_long_long @"_f__void__std/iterator/array-iterator::ArrayIterator<int>__iterate__intarray_long"(ptr, i64)

declare i1 @_mf__ArrayIterator_int__bool__isValid(ptr)

declare ptr @_mf__ArrayIterator_int__intref__get(ptr)

declare i32 @printf(ptr noundef, ...)

declare void @_mp__ArrayIterator_int__void__next(ptr)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
