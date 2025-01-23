; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [11 x i8] c"Result: %d\00", align 1

define private i32 @_Z3getv() {
  %result = alloca i32, align 4
  ret i32 12
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %condition = alloca i1, align 1
  %r = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i1 true, ptr %condition, align 1
  %1 = load i1, ptr %condition, align 1
  br i1 %1, label %cond.true.L7C13, label %cond.false.L7C13

cond.true.L7C13:                                  ; preds = %0
  %2 = call i32 @_Z3getv()
  br label %cond.exit.L7C13

cond.false.L7C13:                                 ; preds = %0
  br label %cond.exit.L7C13

cond.exit.L7C13:                                  ; preds = %cond.false.L7C13, %cond.true.L7C13
  %cond.result = phi i32 [ %2, %cond.true.L7C13 ], [ 24, %cond.false.L7C13 ]
  store i32 %cond.result, ptr %r, align 4
  %3 = load i32, ptr %r, align 4
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %3)
  %5 = load i32, ptr %result, align 4
  ret i32 %5
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
