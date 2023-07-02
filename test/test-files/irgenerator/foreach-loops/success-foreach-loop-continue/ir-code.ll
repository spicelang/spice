; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__short__NumberIterator__short_short_short = type { i16, i16, i16 }
%__long__NumberIterator__long_long_long = type { i64, i64, i64 }

@printf.str.0 = private unnamed_addr constant [10 x i8] c"Short %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [9 x i8] c"Long %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [5 x i8] c"End.\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %shortIterator = alloca %__short__NumberIterator__short_short_short, align 8
  %s = alloca i16, align 2
  %1 = alloca %__long__NumberIterator__long_long_long, align 8
  %l = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  %2 = call %__short__NumberIterator__short_short_short @"_f__void__NumberIterator<short>__range__short_short"(i16 3, i16 8)
  store %__short__NumberIterator__short_short_short %2, ptr %shortIterator, align 2
  store %__short__NumberIterator__short_short_short %2, ptr %shortIterator, align 2
  br label %foreach.head.L5

foreach.head.L5:                                  ; preds = %foreach.tail.L5, %0
  %3 = call i1 @_mf__NumberIterator_short__bool__isValid(ptr %shortIterator)
  br i1 %3, label %foreach.body.L5, label %foreach.exit.L5

foreach.body.L5:                                  ; preds = %foreach.head.L5
  %4 = call ptr @_mf__NumberIterator_short__shortref__get(ptr %shortIterator)
  %5 = load i16, ptr %4, align 2
  store i16 %5, ptr %s, align 2
  %6 = load i16, ptr %s, align 2
  %7 = sext i16 %6 to i32
  %8 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %7)
  %9 = load i16, ptr %s, align 2
  %10 = and i16 %9, 1
  %11 = sext i16 %10 to i32
  %12 = icmp eq i32 %11, 1
  br i1 %12, label %if.then.L7, label %if.exit.L7

if.then.L7:                                       ; preds = %foreach.body.L5
  %13 = call %__long__NumberIterator__long_long_long @"_f__void__NumberIterator<long>__range__long_long"(i64 1, i64 2)
  store %__long__NumberIterator__long_long_long %13, ptr %1, align 8
  br label %foreach.head.L8

foreach.head.L8:                                  ; preds = %foreach.tail.L8, %if.then.L7
  %14 = call i1 @_mf__NumberIterator_long__bool__isValid(ptr %1)
  br i1 %14, label %foreach.body.L8, label %foreach.exit.L8

foreach.body.L8:                                  ; preds = %foreach.head.L8
  %15 = call ptr @_mf__NumberIterator_long__longref__get(ptr %1)
  store ptr %15, ptr %l, align 8
  %16 = load ptr, ptr %l, align 8
  %17 = load i64, ptr %16, align 8
  %18 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i64 %17)
  br label %foreach.tail.L5

foreach.tail.L8:                                  ; No predecessors!
  call void @_mp__NumberIterator_long__void__next(ptr %1)
  br label %foreach.head.L8

foreach.exit.L8:                                  ; preds = %foreach.head.L8
  br label %if.exit.L7

if.exit.L7:                                       ; preds = %foreach.exit.L8, %foreach.body.L5
  br label %foreach.tail.L5

foreach.tail.L5:                                  ; preds = %if.exit.L7, %foreach.body.L8
  call void @_mp__NumberIterator_short__void__next(ptr %shortIterator)
  br label %foreach.head.L5

foreach.exit.L5:                                  ; preds = %foreach.head.L5
  %19 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2)
  %20 = load i32, ptr %result, align 4
  ret i32 %20
}

declare %__short__NumberIterator__short_short_short @"_f__void__NumberIterator<short>__range__short_short"(i16, i16)

declare i1 @_mf__NumberIterator_short__bool__isValid(ptr)

declare ptr @_mf__NumberIterator_short__shortref__get(ptr)

declare i32 @printf(ptr noundef, ...)

declare %__long__NumberIterator__long_long_long @"_f__void__NumberIterator<long>__range__long_long"(i64, i64)

declare i1 @_mf__NumberIterator_long__bool__isValid(ptr)

declare ptr @_mf__NumberIterator_long__longref__get(ptr)

declare void @_mp__NumberIterator_long__void__next(ptr)

declare void @_mp__NumberIterator_short__void__next(ptr)

attributes #0 = { noinline nounwind optnone uwtable }
