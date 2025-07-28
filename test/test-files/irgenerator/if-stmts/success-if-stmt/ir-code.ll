; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [16 x i8] c"Condition true\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [17 x i8] c"Condition false\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %condition = alloca i1, align 1
  store i32 0, ptr %result, align 4
  store i1 true, ptr %condition, align 1
  %1 = load i1, ptr %condition, align 1
  br i1 %1, label %if.then.L3, label %if.exit.L3

if.then.L3:                                       ; preds = %0
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret i32 0

if.exit.L3:                                       ; preds = %0
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1)
  %4 = load i32, ptr %result, align 4
  ret i32 %4
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
