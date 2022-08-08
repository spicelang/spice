; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [20 x i8] c"Fibonacci of %d: %d\00", align 1

; Function Attrs: nofree nosync nounwind readnone
define internal fastcc i32 @_f__void__fib__int(i32 %0) unnamed_addr #0 {
entry.l1:
  %1 = icmp slt i32 %0, 3
  br i1 %1, label %common.ret, label %if.end.l2

common.ret.loopexit:                              ; preds = %if.end.l2
  %phi.bo = add i32 %5, 1
  br label %common.ret

common.ret:                                       ; preds = %common.ret.loopexit, %entry.l1
  %accumulator.tr.lcssa = phi i32 [ 1, %entry.l1 ], [ %phi.bo, %common.ret.loopexit ]
  ret i32 %accumulator.tr.lcssa

if.end.l2:                                        ; preds = %entry.l1, %if.end.l2
  %.tr9 = phi i32 [ %4, %if.end.l2 ], [ %0, %entry.l1 ]
  %accumulator.tr8 = phi i32 [ %5, %if.end.l2 ], [ 0, %entry.l1 ]
  %2 = add nsw i32 %.tr9, -1
  %3 = tail call fastcc i32 @_f__void__fib__int(i32 %2)
  %4 = add nsw i32 %.tr9, -2
  %5 = add i32 %3, %accumulator.tr8
  %6 = icmp ult i32 %.tr9, 5
  br i1 %6, label %common.ret.loopexit, label %if.end.l2
}

; Function Attrs: nofree nounwind
define i32 @main() local_unnamed_addr #1 {
entry.l6:
  %0 = tail call fastcc i32 @_f__void__fib__int(i32 46)
  %1 = tail call i32 (ptr, ...) @printf(ptr nonnull @0, i32 46, i32 %0)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind readnone }
attributes #1 = { nofree nounwind }
