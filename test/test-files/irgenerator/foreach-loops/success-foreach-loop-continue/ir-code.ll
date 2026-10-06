; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.NumberIterator = type { %interface.IIterator, i16, i16, i16 }
%interface.IIterator = type { ptr }
%struct.NumberIterator.0 = type { %interface.IIterator, i64, i64, i64 }

@printf.str.0 = private unnamed_addr constant [10 x i8] c"Short %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [9 x i8] c"Long %d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [5 x i8] c"End.\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %shortIterator = alloca %struct.NumberIterator, align 8
  %s = alloca i16, align 2
  %1 = alloca %struct.NumberIterator.0, align 8
  %l = alloca ptr, align 8
  %2 = alloca ptr, align 8
  call void @_Z5rangeIsE14NumberIteratorIsEss(ptr dead_on_unwind writable sret(%struct.NumberIterator) align 8 %shortIterator, i16 noundef signext 3, i16 noundef signext 8)
  br label %foreach.head.L5

foreach.head.L5:                                  ; preds = %foreach.tail.L5, %0
  %3 = call i1 @_ZN14NumberIteratorIsE7isValidEv(ptr %shortIterator)
  br i1 %3, label %foreach.body.L5, label %foreach.exit.L5

foreach.body.L5:                                  ; preds = %foreach.head.L5
  %4 = call ptr @_ZN14NumberIteratorIsE3getEv(ptr %shortIterator)
  %5 = load i16, ptr %4, align 2
  store i16 %5, ptr %s, align 2
  %6 = load i16, ptr %s, align 2
  %7 = sext i16 %6 to i32
  %8 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %7)
  %9 = load i16, ptr %s, align 2
  %10 = and i16 %9, 1
  %11 = sext i16 %10 to i32
  %12 = icmp eq i32 %11, 1
  br i1 %12, label %if.then.L7, label %if.exit.L7

if.then.L7:                                       ; preds = %foreach.body.L5
  call void @_Z5rangeIlE14NumberIteratorIlEll(ptr dead_on_unwind writable sret(%struct.NumberIterator.0) align 8 %1, i64 noundef 1, i64 noundef 2)
  br label %foreach.head.L8

foreach.head.L8:                                  ; preds = %foreach.tail.L8, %if.then.L7
  %13 = call i1 @_ZN14NumberIteratorIlE7isValidEv(ptr %1)
  br i1 %13, label %foreach.body.L8, label %foreach.exit.L8

foreach.body.L8:                                  ; preds = %foreach.head.L8
  %14 = call ptr @_ZN14NumberIteratorIlE3getEv(ptr %1)
  store ptr %14, ptr %2, align 8
  %15 = load ptr, ptr %2, align 8
  %16 = load i64, ptr %15, align 8
  %17 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i64 noundef %16)
  br label %foreach.tail.L5

foreach.tail.L8:                                  ; No predecessors!
  call void @_ZN14NumberIteratorIlE4nextEv(ptr %1)
  br label %foreach.head.L8

foreach.exit.L8:                                  ; preds = %foreach.head.L8
  br label %if.exit.L7

if.exit.L7:                                       ; preds = %foreach.exit.L8, %foreach.body.L5
  br label %foreach.tail.L5

foreach.tail.L5:                                  ; preds = %if.exit.L7, %foreach.body.L8
  call void @_ZN14NumberIteratorIsE4nextEv(ptr %shortIterator)
  br label %foreach.head.L5

foreach.exit.L5:                                  ; preds = %foreach.head.L5
  %18 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2)
  ret i32 0
}

declare void @_Z5rangeIsE14NumberIteratorIsEss(ptr dead_on_unwind noalias writable sret(%struct.NumberIterator) align 8, i16, i16)

declare i1 @_ZN14NumberIteratorIsE7isValidEv(ptr)

declare ptr @_ZN14NumberIteratorIsE3getEv(ptr)

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

declare void @_Z5rangeIlE14NumberIteratorIlEll(ptr dead_on_unwind noalias writable sret(%struct.NumberIterator.0) align 8, i64, i64)

declare i1 @_ZN14NumberIteratorIlE7isValidEv(ptr)

declare ptr @_ZN14NumberIteratorIlE3getEv(ptr)

declare void @_ZN14NumberIteratorIlE4nextEv(ptr)

declare void @_ZN14NumberIteratorIsE4nextEv(ptr)

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
