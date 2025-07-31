; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.ArrayIterator = type { %interface.IIterator, ptr, i64, i64 }
%interface.IIterator = type { ptr }
%struct.Pair = type { i64, ptr }

@anon.array.0 = private unnamed_addr constant [7 x i32] [i32 1, i32 5, i32 4, i32 0, i32 12, i32 12345, i32 9]
@printf.str.0 = private unnamed_addr constant [23 x i8] c"Item for index %d, %d\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %intArray = alloca [7 x i32], align 4
  %1 = alloca %struct.ArrayIterator, align 8
  %index = alloca i64, align 8
  %item = alloca i32, align 4
  %pair.addr = alloca %struct.Pair, align 8
  store i32 0, ptr %result, align 4
  store [7 x i32] [i32 1, i32 5, i32 4, i32 0, i32 12, i32 12345, i32 9], ptr %intArray, align 4
  %2 = getelementptr inbounds [7 x i32], ptr %intArray, i64 0, i32 0
  %3 = call %struct.ArrayIterator @_Z7iterateIiE13ArrayIteratorIiEPim(ptr %2, i64 7)
  store %struct.ArrayIterator %3, ptr %1, align 8
  store i64 0, ptr %index, align 8
  br label %foreach.head.L5

foreach.head.L5:                                  ; preds = %foreach.tail.L5, %0
  %4 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr %1)
  br i1 %4, label %foreach.body.L5, label %foreach.exit.L5

foreach.body.L5:                                  ; preds = %foreach.head.L5
  %pair = call %struct.Pair @_ZN13ArrayIteratorIiE6getIdxEv(ptr %1)
  store %struct.Pair %pair, ptr %pair.addr, align 8
  %5 = load i64, ptr %pair.addr, align 8
  store i64 %5, ptr %index, align 8
  %item.addr = getelementptr inbounds nuw %struct.Pair, ptr %pair.addr, i32 0, i32 1
  %6 = load ptr, ptr %item.addr, align 8
  %7 = load i64, ptr %index, align 8
  %8 = load i32, ptr %6, align 4
  %9 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i64 %7, i32 %8)
  br label %foreach.tail.L5

foreach.tail.L5:                                  ; preds = %foreach.body.L5
  call void @_ZN13ArrayIteratorIiE4nextEv(ptr %1)
  br label %foreach.head.L5

foreach.exit.L5:                                  ; preds = %foreach.head.L5
  %10 = load i32, ptr %result, align 4
  ret i32 %10
}

declare %struct.ArrayIterator @_Z7iterateIiE13ArrayIteratorIiEPim(ptr, i64)

declare i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr)

declare %struct.Pair @_ZN13ArrayIteratorIiE6getIdxEv(ptr)

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) #1

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
