; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [11 x i8] c"Result: %d\00", align 4

; Function Attrs: nofree nosync nounwind memory(none)
define private fastcc i32 @_Z4fiboi(i32 %0) unnamed_addr #0 {
  %2 = icmp slt i32 %0, 2
  br i1 %2, label %common.ret, label %if.exit.L2

common.ret:                                       ; preds = %if.exit.L2, %1
  %accumulator.tr.lcssa = phi i32 [ 0, %1 ], [ %6, %if.exit.L2 ]
  %.tr.lcssa = phi i32 [ %0, %1 ], [ %5, %if.exit.L2 ]
  %accumulator.ret.tr = add nsw i32 %.tr.lcssa, %accumulator.tr.lcssa
  ret i32 %accumulator.ret.tr

if.exit.L2:                                       ; preds = %1, %if.exit.L2
  %.tr5 = phi i32 [ %5, %if.exit.L2 ], [ %0, %1 ]
  %accumulator.tr4 = phi i32 [ %6, %if.exit.L2 ], [ 0, %1 ]
  %3 = add nsw i32 %.tr5, -1
  %4 = tail call fastcc i32 @_Z4fiboi(i32 %3)
  %5 = add nsw i32 %.tr5, -2
  %6 = add nsw i32 %4, %accumulator.tr4
  %7 = icmp samesign ult i32 %.tr5, 4
  br i1 %7, label %common.ret, label %if.exit.L2
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() local_unnamed_addr #1 {
  %1 = tail call fastcc i32 @_Z4fiboi(i32 30) #3
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 %1)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

attributes #0 = { nofree nosync nounwind memory(none) }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
