; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [36 x i8] c"Ackermann of base m=%d and n=%d: %d\00", align 1

; Function Attrs: nofree nosync nounwind readnone
define internal fastcc i32 @_f__void__int__ack__int_int(i32 %0, i32 %1) unnamed_addr #0 {
entry.l1:
  %2 = icmp eq i32 %0, 0
  br i1 %2, label %if.then.l2, label %if.end.l2

if.then.l2:                                       ; preds = %tailrecurse.backedge, %entry.l1
  %.tr15.lcssa = phi i32 [ %1, %entry.l1 ], [ %.tr15.be, %tailrecurse.backedge ]
  %3 = add i32 %.tr15.lcssa, 1
  ret i32 %3

if.end.l2:                                        ; preds = %entry.l1, %tailrecurse.backedge
  %.tr1517 = phi i32 [ %.tr15.be, %tailrecurse.backedge ], [ %1, %entry.l1 ]
  %.tr16 = phi i32 [ %.tr.be, %tailrecurse.backedge ], [ %0, %entry.l1 ]
  %4 = icmp eq i32 %.tr1517, 0
  br i1 %4, label %tailrecurse.backedge, label %if.end.l3

tailrecurse.backedge:                             ; preds = %if.end.l2, %if.end.l3
  %.tr15.be = phi i32 [ %7, %if.end.l3 ], [ 1, %if.end.l2 ]
  %.tr.be = add i32 %.tr16, -1
  %5 = icmp eq i32 %.tr.be, 0
  br i1 %5, label %if.then.l2, label %if.end.l2

if.end.l3:                                        ; preds = %if.end.l2
  %6 = add i32 %.tr1517, -1
  %7 = tail call fastcc i32 @_f__void__int__ack__int_int(i32 %.tr16, i32 %6)
  br label %tailrecurse.backedge
}

; Function Attrs: nofree nounwind
define i32 @main() local_unnamed_addr #1 {
entry.l7:
  %0 = tail call fastcc i32 @_f__void__int__ack__int_int(i32 3, i32 12)
  %1 = tail call i32 (ptr, ...) @printf(ptr nonnull @0, i32 3, i32 12, i32 %0)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind readnone }
attributes #1 = { nofree nounwind }
