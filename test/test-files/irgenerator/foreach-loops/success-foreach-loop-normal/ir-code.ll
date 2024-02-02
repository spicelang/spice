; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.ArrayIterator = type { %interface.IIterator, ptr, i64, i64 }
%interface.IIterator = type { ptr }

@anon.array.0 = private unnamed_addr constant [7 x i32] [i32 1, i32 5, i32 4, i32 0, i32 12, i32 12345, i32 9]
@printf.str.0 = private unnamed_addr constant [10 x i8] c"Item: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %intArray = alloca [7 x i32], align 4
  %1 = alloca %struct.ArrayIterator, align 8
  %item = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store [7 x i32] [i32 1, i32 5, i32 4, i32 0, i32 12, i32 12345, i32 9], ptr %intArray, align 4
  %2 = getelementptr inbounds [7 x i32], ptr %intArray, i32 0, i32 0
  %3 = call %struct.ArrayIterator @_Z7iterateIiE13ArrayIteratorIiEPim(ptr %2, i64 7)
  store %struct.ArrayIterator %3, ptr %1, align 8
  br label %foreach.head.L5

foreach.head.L5:                                  ; preds = %foreach.tail.L5, %0
  %4 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr %1)
  br i1 %4, label %foreach.body.L5, label %foreach.exit.L5

foreach.body.L5:                                  ; preds = %foreach.head.L5
  %5 = call ptr @_ZN13ArrayIteratorIiE3getEv(ptr %1)
  %6 = load i32, ptr %5, align 4
  store i32 %6, ptr %item, align 4
  %7 = load i32, ptr %item, align 4
  %8 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %7)
  br label %foreach.tail.L5

foreach.tail.L5:                                  ; preds = %foreach.body.L5
  call void @_ZN13ArrayIteratorIiE4nextEv(ptr %1)
  br label %foreach.head.L5

foreach.exit.L5:                                  ; preds = %foreach.head.L5
  %9 = load i32, ptr %result, align 4
  ret i32 %9
}

declare %struct.ArrayIterator @_Z7iterateIiE13ArrayIteratorIiEPim(ptr, i64)

declare i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr)

declare ptr @_ZN13ArrayIteratorIiE3getEv(ptr)

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

declare void @_ZN13ArrayIteratorIiE4nextEv(ptr)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
