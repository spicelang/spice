; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [36 x i8] c"Ackermann of base m=%d and n=%d: %d\00", align 1

; Function Attrs: nofree nosync nounwind memory(none)
define private fastcc i32 @_Z3ackii(i32 %0, i32 %1) unnamed_addr #0 {
  %3 = icmp eq i32 %0, 0
  br i1 %3, label %if.then.L2, label %if.exit.L2

if.then.L2:                                       ; preds = %tailrecurse.backedge, %2
  %.tr7.lcssa = phi i32 [ %1, %2 ], [ %.tr7.be, %tailrecurse.backedge ]
  %4 = add nsw i32 %.tr7.lcssa, 1
  ret i32 %4

if.exit.L2:                                       ; preds = %2, %tailrecurse.backedge
  %.tr79 = phi i32 [ %.tr7.be, %tailrecurse.backedge ], [ %1, %2 ]
  %.tr8 = phi i32 [ %.tr.be, %tailrecurse.backedge ], [ %0, %2 ]
  %5 = icmp eq i32 %.tr79, 0
  br i1 %5, label %tailrecurse.backedge, label %if.exit.L3

tailrecurse.backedge:                             ; preds = %if.exit.L2, %if.exit.L3
  %.tr7.be = phi i32 [ %8, %if.exit.L3 ], [ 1, %if.exit.L2 ]
  %.tr.be = add nsw i32 %.tr8, -1
  %6 = icmp eq i32 %.tr.be, 0
  br i1 %6, label %if.then.L2, label %if.exit.L2

if.exit.L3:                                       ; preds = %if.exit.L2
  %7 = add nsw i32 %.tr79, -1
  %8 = tail call fastcc i32 @_Z3ackii(i32 %.tr8, i32 %7)
  br label %tailrecurse.backedge
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #1 {
  %1 = tail call fastcc i32 @_Z3ackii(i32 3, i32 10) #3
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 3, i32 10, i32 %1)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #2

attributes #0 = { nofree nosync nounwind memory(none) }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
attributes #3 = { nounwind }
