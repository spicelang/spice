; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.ArrayIterator = type { %interface.IIterator, ptr, i64, i64 }
%interface.IIterator = type { ptr }

@anon.array.0 = private unnamed_addr constant [7 x i32] [i32 1, i32 5, i32 4, i32 0, i32 12, i32 12345, i32 9]
@printf.str.0 = private unnamed_addr constant [10 x i8] c"Item: %d\0A\00", align 4
@anon.array.1 = private unnamed_addr constant [7 x i32] [i32 1, i32 5, i32 4, i32 0, i32 12, i32 12345, i32 9]
@printf.str.1 = private unnamed_addr constant [10 x i8] c"Item: %d\0A\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %1 = alloca %struct.ArrayIterator, align 8
  %item = alloca i32, align 4
  %array = alloca [7 x i32], align 4
  %2 = alloca %struct.ArrayIterator, align 8
  %item1 = alloca i32, align 4
  call void @_Z7iterateIiE13ArrayIteratorIiEPim(ptr dead_on_unwind writable sret(%struct.ArrayIterator) align 8 %1, ptr @anon.array.0, i64 7)
  br label %foreach.head.L4

foreach.head.L4:                                  ; preds = %foreach.tail.L4, %0
  %3 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr %1)
  br i1 %3, label %foreach.body.L4, label %foreach.exit.L4

foreach.body.L4:                                  ; preds = %foreach.head.L4
  %4 = call ptr @_ZN13ArrayIteratorIiE3getEv(ptr %1)
  %5 = load i32, ptr %4, align 4
  store i32 %5, ptr %item, align 4
  %6 = load i32, ptr %item, align 4
  %7 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %6)
  br label %foreach.tail.L4

foreach.tail.L4:                                  ; preds = %foreach.body.L4
  call void @_ZN13ArrayIteratorIiE4nextEv(ptr %1)
  br label %foreach.head.L4

foreach.exit.L4:                                  ; preds = %foreach.head.L4
  store [7 x i32] [i32 1, i32 5, i32 4, i32 0, i32 12, i32 12345, i32 9], ptr %array, align 4
  call void @_Z7iterateIiE13ArrayIteratorIiEPim(ptr dead_on_unwind writable sret(%struct.ArrayIterator) align 8 %2, ptr %array, i64 7)
  br label %foreach.head.L8

foreach.head.L8:                                  ; preds = %foreach.tail.L8, %foreach.exit.L4
  %8 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr %2)
  br i1 %8, label %foreach.body.L8, label %foreach.exit.L8

foreach.body.L8:                                  ; preds = %foreach.head.L8
  %9 = call ptr @_ZN13ArrayIteratorIiE3getEv(ptr %2)
  %10 = load i32, ptr %9, align 4
  store i32 %10, ptr %item1, align 4
  %11 = load i32, ptr %item1, align 4
  %12 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %11)
  br label %foreach.tail.L8

foreach.tail.L8:                                  ; preds = %foreach.body.L8
  call void @_ZN13ArrayIteratorIiE4nextEv(ptr %2)
  br label %foreach.head.L8

foreach.exit.L8:                                  ; preds = %foreach.head.L8
  ret i32 0
}

declare void @_Z7iterateIiE13ArrayIteratorIiEPim(ptr dead_on_unwind noalias writable sret(%struct.ArrayIterator) align 8, ptr, i64)

declare i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr)

declare ptr @_ZN13ArrayIteratorIiE3getEv(ptr)

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

declare void @_ZN13ArrayIteratorIiE4nextEv(ptr)

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
