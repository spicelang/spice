; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [36 x i8] c"Ackermann of base m=%d and n=%d: %d\00", align 1

; Function Attrs: nofree nosync nounwind readnone
define internal fastcc i32 @"ack(int,int)"(i32 %0, i32 %1) unnamed_addr #0 {
entry:
  %2 = icmp eq i32 %0, 0
  br i1 %2, label %if.then, label %if.end

if.then:                                          ; preds = %tailrecurse.backedge, %entry
  %.tr23.lcssa = phi i32 [ %1, %entry ], [ %.tr23.be, %tailrecurse.backedge ]
  %3 = add i32 %.tr23.lcssa, 1
  ret i32 %3

if.end:                                           ; preds = %entry, %tailrecurse.backedge
  %.tr2325 = phi i32 [ %.tr23.be, %tailrecurse.backedge ], [ %1, %entry ]
  %.tr24 = phi i32 [ %.tr.be, %tailrecurse.backedge ], [ %0, %entry ]
  %4 = icmp eq i32 %.tr2325, 0
  br i1 %4, label %tailrecurse.backedge, label %if.end2

tailrecurse.backedge:                             ; preds = %if.end, %if.end2
  %.tr23.be = phi i32 [ %7, %if.end2 ], [ 1, %if.end ]
  %.tr.be = add i32 %.tr24, -1
  %5 = icmp eq i32 %.tr.be, 0
  br i1 %5, label %if.then, label %if.end

if.end2:                                          ; preds = %if.end
  %6 = add i32 %.tr2325, -1
  %7 = tail call fastcc i32 @"ack(int,int)"(i32 %.tr24, i32 %6)
  br label %tailrecurse.backedge
}

; Function Attrs: nofree nounwind
define i32 @main() local_unnamed_addr #1 {
entry:
  %0 = tail call fastcc i32 @"ack(int,int)"(i32 3, i32 12)
  %1 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([36 x i8], [36 x i8]* @0, i64 0, i64 0), i32 3, i32 12, i32 %0)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(i8* nocapture noundef readonly, ...) local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind readnone }
attributes #1 = { nofree nounwind }
