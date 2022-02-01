; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [20 x i8] c"Fibonacci of %d: %d\00", align 1

declare i32 @printf(i8*, ...)

; Function Attrs: nounwind
define i32 @"fib(int)"(i32 %0) #0 {
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
  %3 = tail call i32 @"fib(int)"(i32 %2)
  %4 = add nsw i32 %.tr12, -2
  %5 = add i32 %3, %accumulator.tr11
  %6 = icmp slt i32 %.tr12, 5
  br i1 %6, label %common.ret.loopexit, label %if.end
}

define i32 @main() {
entry:
  %0 = tail call i32 @"fib(int)"(i32 46)
  %1 = tail call i32 (i8*, ...) @printf(i8* noundef nonnull dereferenceable(1) getelementptr inbounds ([20 x i8], [20 x i8]* @0, i64 0, i64 0), i32 46, i32 %0)
  ret i32 0
}

attributes #0 = { nounwind }
