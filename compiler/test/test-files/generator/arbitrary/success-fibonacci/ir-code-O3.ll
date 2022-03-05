; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [20 x i8] c"Fibonacci of %d: %d\00", align 1

; Function Attrs: nofree nounwind
declare noundef i32 @printf(i8* nocapture noundef readonly, ...) local_unnamed_addr #0

; Function Attrs: nofree nosync nounwind readnone
define internal fastcc i32 @"fib(int)"(i32 %0) unnamed_addr #1 {
entry:
  %1 = icmp slt i32 %0, 3
  br i1 %1, label %common.ret, label %if.end

common.ret.loopexit:                              ; preds = %if.end
  %phi.bo = add i32 %5, 1
  br label %common.ret

common.ret:                                       ; preds = %common.ret.loopexit, %entry
  %accumulator.tr.lcssa = phi i32 [ 1, %entry ], [ %phi.bo, %common.ret.loopexit ]
  ret i32 %accumulator.tr.lcssa

if.end:                                           ; preds = %entry, %if.end
  %.tr12 = phi i32 [ %4, %if.end ], [ %0, %entry ]
  %accumulator.tr11 = phi i32 [ %5, %if.end ], [ 0, %entry ]
  %2 = add nsw i32 %.tr12, -1
  %3 = tail call fastcc i32 @"fib(int)"(i32 %2)
  %4 = add nsw i32 %.tr12, -2
  %5 = add i32 %3, %accumulator.tr11
  %6 = icmp ult i32 %.tr12, 5
  br i1 %6, label %common.ret.loopexit, label %if.end
}

; Function Attrs: nofree nounwind
define i32 @main() local_unnamed_addr #0 {
entry:
  %0 = tail call fastcc i32 @"fib(int)"(i32 46)
  %1 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([20 x i8], [20 x i8]* @0, i64 0, i64 0), i32 46, i32 %0)
  ret i32 0
}

attributes #0 = { nofree nounwind }
attributes #1 = { nofree nosync nounwind readnone }
