; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [21 x i8] c"Faculty of %d is: %d\00", align 1

declare i32 @printf(i8*, ...)

; Function Attrs: nounwind
define i32 @"faculty(int)"(i32 %0) #0 {
entry:
  %1 = icmp slt i32 %0, 2
  br i1 %1, label %common.ret, label %if.end

common.ret:                                       ; preds = %if.end, %entry
  %accumulator.tr.lcssa = phi i32 [ 1, %entry ], [ %3, %if.end ]
  ret i32 %accumulator.tr.lcssa

if.end:                                           ; preds = %entry, %if.end
  %.tr9 = phi i32 [ %2, %if.end ], [ %0, %entry ]
  %accumulator.tr8 = phi i32 [ %3, %if.end ], [ 1, %entry ]
  %2 = add nsw i32 %.tr9, -1
  %3 = mul i32 %.tr9, %accumulator.tr8
  %4 = icmp slt i32 %.tr9, 3
  br i1 %4, label %common.ret, label %if.end
}

define i32 @main() {
entry:
  %0 = tail call i32 @"faculty(int)"(i32 10)
  %1 = tail call i32 (i8*, ...) @printf(i8* noundef nonnull dereferenceable(1) getelementptr inbounds ([21 x i8], [21 x i8]* @0, i64 0, i64 0), i32 10, i32 %0)
  ret i32 0
}

attributes #0 = { nounwind }
