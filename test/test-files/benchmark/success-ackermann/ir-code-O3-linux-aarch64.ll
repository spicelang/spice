; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [36 x i8] c"Ackermann of base m=%d and n=%d: %d\00", align 4

; Function Attrs: nofree nosync nounwind memory(none)
define private fastcc range(i32 -2147483647, -2147483648) i32 @_Z3ackii(i32 range(i32 0, 4) %0, i32 %1) unnamed_addr #0 {
  %3 = icmp eq i32 %0, 0
  br i1 %3, label %if.then.L2, label %if.exit.L2

if.then.L2:                                       ; preds = %if.exit.L2.2, %if.exit.L3.2, %tailrecurse.backedge, %tailrecurse.backedge.1, %2
  %.tr7.lcssa = phi i32 [ %1, %2 ], [ %.tr7.be, %tailrecurse.backedge ], [ %.tr7.be.1, %tailrecurse.backedge.1 ], [ %13, %if.exit.L3.2 ], [ 1, %if.exit.L2.2 ]
  %4 = add nsw i32 %.tr7.lcssa, 1
  ret i32 %4

if.exit.L2:                                       ; preds = %2
  %5 = icmp eq i32 %1, 0
  br i1 %5, label %tailrecurse.backedge, label %if.exit.L3

tailrecurse.backedge:                             ; preds = %if.exit.L2, %if.exit.L3
  %.tr7.be = phi i32 [ %15, %if.exit.L3 ], [ 1, %if.exit.L2 ]
  %.tr.be = add nsw i32 %0, -1
  %6 = icmp eq i32 %.tr.be, 0
  br i1 %6, label %if.then.L2, label %if.exit.L2.1

if.exit.L2.1:                                     ; preds = %tailrecurse.backedge
  %7 = icmp eq i32 %.tr7.be, 0
  br i1 %7, label %tailrecurse.backedge.1, label %if.exit.L3.1

if.exit.L3.1:                                     ; preds = %if.exit.L2.1
  %8 = add nsw i32 %.tr7.be, -1
  %9 = tail call fastcc i32 @_Z3ackii(i32 %.tr.be, i32 %8)
  br label %tailrecurse.backedge.1

tailrecurse.backedge.1:                           ; preds = %if.exit.L3.1, %if.exit.L2.1
  %.tr7.be.1 = phi i32 [ %9, %if.exit.L3.1 ], [ 1, %if.exit.L2.1 ]
  %.tr.be.1 = add nsw i32 %0, -2
  %10 = icmp eq i32 %.tr.be.1, 0
  br i1 %10, label %if.then.L2, label %if.exit.L2.2

if.exit.L2.2:                                     ; preds = %tailrecurse.backedge.1
  %11 = icmp eq i32 %.tr7.be.1, 0
  br i1 %11, label %if.then.L2, label %if.exit.L3.2

if.exit.L3.2:                                     ; preds = %if.exit.L2.2
  %12 = add nsw i32 %.tr7.be.1, -1
  %13 = tail call fastcc i32 @_Z3ackii(i32 %.tr.be.1, i32 %12)
  br label %if.then.L2

if.exit.L3:                                       ; preds = %if.exit.L2
  %14 = add nsw i32 %1, -1
  %15 = tail call fastcc i32 @_Z3ackii(i32 %0, i32 %14)
  br label %tailrecurse.backedge
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #1 {
  %1 = tail call fastcc i32 @_Z3ackii(i32 3, i32 10) #3
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 3, i32 10, i32 %1)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

attributes #0 = { nofree nosync nounwind memory(none) }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
