; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [55 x i8] c"Assertion failed: Condition 'true' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [26 x i8] c"First assertion was true\0A\00", align 1
@anon.string.1 = private unnamed_addr constant [57 x i8] c"Assertion failed: Condition '1 != 1' evaluated to false.\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  br i1 true, label %assert.exit.L2, label %assert.then.L2, !prof !5

assert.then.L2:                                   ; preds = %0
  %1 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L2:                                   ; preds = %0
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  br i1 false, label %assert.exit.L5, label %assert.then.L5, !prof !5

assert.then.L5:                                   ; preds = %assert.exit.L2
  %3 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L5:                                   ; preds = %assert.exit.L2
  %4 = load i32, ptr %result, align 4
  ret i32 %4
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #2

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { cold noreturn nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = !{!"branch_weights", i32 2000, i32 1}
