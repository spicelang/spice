; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [12 x i8] c"F1 called.\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [12 x i8] c"F2 called.\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [11 x i8] c"Result: %d\00", align 4

define private i1 @_Z2f1v() {
  %result = alloca i1, align 1
  %1 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret i1 false
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) #0

define private i1 @_Z2f2v() {
  %result = alloca i1, align 1
  %1 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1)
  ret i1 true
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call i1 @_Z2f1v()
  br i1 %1, label %cond.true.L12C26, label %cond.false.L12C26

cond.true.L12C26:                                 ; preds = %0
  %2 = call i1 @_Z2f1v()
  br label %cond.exit.L12C26

cond.false.L12C26:                                ; preds = %0
  %3 = call i1 @_Z2f2v()
  br label %cond.exit.L12C26

cond.exit.L12C26:                                 ; preds = %cond.false.L12C26, %cond.true.L12C26
  %cond.result = phi i1 [ %2, %cond.true.L12C26 ], [ %3, %cond.false.L12C26 ]
  %4 = zext i1 %cond.result to i32
  %5 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 %4)
  %6 = load i32, ptr %result, align 4
  ret i32 %6
}

attributes #0 = { nofree nounwind }
attributes #1 = { noinline nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
