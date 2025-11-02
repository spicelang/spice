; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [12 x i8] c"Result: %d\0A\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %condA = alloca i1, align 1
  %condB = alloca i1, align 1
  %condC = alloca i1, align 1
  %condD = alloca i1, align 1
  store i32 0, ptr %result, align 4
  store i1 true, ptr %condA, align 1
  store i1 false, ptr %condB, align 1
  store i1 true, ptr %condC, align 1
  store i1 false, ptr %condD, align 1
  %1 = load i1, ptr %condA, align 1
  br i1 %1, label %cond.true.L6C14, label %cond.false.L6C14

cond.true.L6C14:                                  ; preds = %0
  %2 = load i1, ptr %condB, align 1
  br i1 %2, label %cond.true.L6C23, label %cond.false.L6C23

cond.true.L6C23:                                  ; preds = %cond.true.L6C14
  br label %cond.exit.L6C23

cond.false.L6C23:                                 ; preds = %cond.true.L6C14
  %3 = load i1, ptr %condC, align 1
  br i1 %3, label %cond.true.L6C36, label %cond.false.L6C36

cond.true.L6C36:                                  ; preds = %cond.false.L6C23
  %4 = load i1, ptr %condD, align 1
  %5 = select i1 %4, i32 4, i32 0
  br label %cond.exit.L6C36

cond.false.L6C36:                                 ; preds = %cond.false.L6C23
  br label %cond.exit.L6C36

cond.exit.L6C36:                                  ; preds = %cond.false.L6C36, %cond.true.L6C36
  %cond.result = phi i32 [ %5, %cond.true.L6C36 ], [ 3, %cond.false.L6C36 ]
  br label %cond.exit.L6C23

cond.exit.L6C23:                                  ; preds = %cond.exit.L6C36, %cond.true.L6C23
  %cond.result1 = phi i32 [ 2, %cond.true.L6C23 ], [ %cond.result, %cond.exit.L6C36 ]
  br label %cond.exit.L6C14

cond.false.L6C14:                                 ; preds = %0
  br label %cond.exit.L6C14

cond.exit.L6C14:                                  ; preds = %cond.false.L6C14, %cond.exit.L6C23
  %cond.result2 = phi i32 [ %cond.result1, %cond.exit.L6C23 ], [ 1, %cond.false.L6C14 ]
  store i32 %cond.result2, ptr %result, align 4
  %6 = load i32, ptr %result, align 4
  %7 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %6)
  %8 = load i32, ptr %result, align 4
  ret i32 %8
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
