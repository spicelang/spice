; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.NumberIterator = type { %interface.IIterator, i16, i16, i16 }
%interface.IIterator = type { ptr }
%struct.NumberIterator.0 = type { %interface.IIterator, i64, i64, i64 }

@printf.str.0 = private unnamed_addr constant [10 x i8] c"Short %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [9 x i8] c"Long %d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [5 x i8] c"End.\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind uwtable
define dso_local noundef i32 @main() local_unnamed_addr #0 {
  %shortIterator = alloca %struct.NumberIterator, align 8
  %1 = alloca %struct.NumberIterator.0, align 8
  call void @_Z5rangeIsE14NumberIteratorIsEss(ptr dead_on_unwind nonnull writable sret(%struct.NumberIterator) align 8 %shortIterator, i16 noundef signext 3, i16 noundef signext 8) #2
  %2 = call i1 @_ZN14NumberIteratorIsE7isValidEv(ptr nonnull %shortIterator) #2
  br i1 %2, label %foreach.body.L5, label %foreach.exit.L5

foreach.body.L5:                                  ; preds = %0, %foreach.tail.L5
  %3 = call ptr @_ZN14NumberIteratorIsE3getEv(ptr nonnull %shortIterator) #2
  %4 = load i16, ptr %3, align 2
  %5 = sext i16 %4 to i32
  %6 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 noundef %5)
  %7 = and i16 %4, 1
  %.not = icmp eq i16 %7, 0
  br i1 %.not, label %foreach.tail.L5, label %if.then.L7

if.then.L7:                                       ; preds = %foreach.body.L5
  call void @_Z5rangeIlE14NumberIteratorIlEll(ptr dead_on_unwind nonnull writable sret(%struct.NumberIterator.0) align 8 %1, i64 noundef 1, i64 noundef 2) #2
  %8 = call i1 @_ZN14NumberIteratorIlE7isValidEv(ptr nonnull %1) #2
  br i1 %8, label %foreach.body.L8, label %foreach.tail.L5

foreach.body.L8:                                  ; preds = %if.then.L7
  %9 = call ptr @_ZN14NumberIteratorIlE3getEv(ptr nonnull %1) #2
  %10 = load i64, ptr %9, align 8
  %11 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i64 noundef %10)
  br label %foreach.exit.L5

foreach.tail.L5:                                  ; preds = %foreach.body.L5, %if.then.L7
  call void @_ZN14NumberIteratorIsE4nextEv(ptr nonnull %shortIterator) #2
  %12 = call i1 @_ZN14NumberIteratorIsE7isValidEv(ptr nonnull %shortIterator) #2
  br i1 %12, label %foreach.body.L5, label %foreach.exit.L5

foreach.exit.L5:                                  ; preds = %foreach.tail.L5, %0, %foreach.body.L8
  %13 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2)
  ret i32 0
}

declare void @_Z5rangeIsE14NumberIteratorIsEss(ptr dead_on_unwind noalias writable sret(%struct.NumberIterator) align 8, i16, i16) local_unnamed_addr

declare i1 @_ZN14NumberIteratorIsE7isValidEv(ptr) local_unnamed_addr

declare ptr @_ZN14NumberIteratorIsE3getEv(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

declare void @_Z5rangeIlE14NumberIteratorIlEll(ptr dead_on_unwind noalias writable sret(%struct.NumberIterator.0) align 8, i64, i64) local_unnamed_addr

declare i1 @_ZN14NumberIteratorIlE7isValidEv(ptr) local_unnamed_addr

declare ptr @_ZN14NumberIteratorIlE3getEv(ptr) local_unnamed_addr

declare void @_ZN14NumberIteratorIsE4nextEv(ptr) local_unnamed_addr

attributes #0 = { mustprogress noinline norecurse nounwind uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev [self-hosted] (https://github.com/spicelang/spice)"}
