; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.ArrayIterator = type { %interface.IIterator, ptr, i64, i64 }
%interface.IIterator = type { ptr }

@anon.array.0 = private unnamed_addr constant [7 x i32] [i32 1, i32 5, i32 4, i32 0, i32 12, i32 12345, i32 9]
@printf.str.0 = private unnamed_addr constant [10 x i8] c"Item: %d\0A\00", align 1
@anon.array.1 = private unnamed_addr constant [7 x i32] [i32 1, i32 5, i32 4, i32 0, i32 12, i32 12345, i32 9]
@printf.str.1 = private unnamed_addr constant [10 x i8] c"Item: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %1 = alloca %struct.ArrayIterator, align 8
  %item = alloca i32, align 4
  %array = alloca [7 x i32], align 4
  %2 = alloca %struct.ArrayIterator, align 8
  %item1 = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %3 = call %struct.ArrayIterator @_Z7iterateIiE13ArrayIteratorIiEPim(ptr @anon.array.0, i64 7)
  store %struct.ArrayIterator %3, ptr %1, align 8
  br label %foreach.head.L4

foreach.head.L4:                                  ; preds = %foreach.tail.L4, %0
  %4 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr %1)
  br i1 %4, label %foreach.body.L4, label %foreach.exit.L4

foreach.body.L4:                                  ; preds = %foreach.head.L4
  %5 = call ptr @_ZN13ArrayIteratorIiE3getEv(ptr %1)
  %6 = load i32, ptr %5, align 4
  store i32 %6, ptr %item, align 4
  %7 = load i32, ptr %item, align 4
  %8 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %7)
  br label %foreach.tail.L4

foreach.tail.L4:                                  ; preds = %foreach.body.L4
  call void @_ZN13ArrayIteratorIiE4nextEv(ptr %1)
  br label %foreach.head.L4

foreach.exit.L4:                                  ; preds = %foreach.head.L4
  store [7 x i32] [i32 1, i32 5, i32 4, i32 0, i32 12, i32 12345, i32 9], ptr %array, align 4
  %9 = call %struct.ArrayIterator @_Z7iterateIiE13ArrayIteratorIiEPim(ptr %array, i64 7)
  store %struct.ArrayIterator %9, ptr %2, align 8
  br label %foreach.head.L8

foreach.head.L8:                                  ; preds = %foreach.tail.L8, %foreach.exit.L4
  %10 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr %2)
  br i1 %10, label %foreach.body.L8, label %foreach.exit.L8

foreach.body.L8:                                  ; preds = %foreach.head.L8
  %11 = call ptr @_ZN13ArrayIteratorIiE3getEv(ptr %2)
  %12 = load i32, ptr %11, align 4
  store i32 %12, ptr %item1, align 4
  %13 = load i32, ptr %item1, align 4
  %14 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %13)
  br label %foreach.tail.L8

foreach.tail.L8:                                  ; preds = %foreach.body.L8
  call void @_ZN13ArrayIteratorIiE4nextEv(ptr %2)
  br label %foreach.head.L8

foreach.exit.L8:                                  ; preds = %foreach.head.L8
  %15 = load i32, ptr %result, align 4
  ret i32 %15
}

declare %struct.ArrayIterator @_Z7iterateIiE13ArrayIteratorIiEPim(ptr, i64)

declare i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr)

declare ptr @_ZN13ArrayIteratorIiE3getEv(ptr)

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

declare void @_ZN13ArrayIteratorIiE4nextEv(ptr)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
