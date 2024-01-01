; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [11 x i8] c"Result: %d\00", align 1

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
  %7 = icmp ult i32 %.tr5, 4
  br i1 %7, label %common.ret, label %if.exit.L2
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #1 {
  %1 = tail call fastcc i32 @_Z4fiboi(i32 30) #3
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 %1)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #2

attributes #0 = { nofree nosync nounwind memory(none) }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
attributes #3 = { nounwind }
