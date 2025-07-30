; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [10 x i8] c"Argc: %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [16 x i8] c"Argv no. 0: %s\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [16 x i8] c"Argv no. 1: %s\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 %0, ptr %1) #0 {
  %result = alloca i32, align 4
  %argc = alloca i32, align 4
  %argv = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  store i32 %0, ptr %argc, align 4
  store ptr %1, ptr %argv, align 8
  %3 = load i32, ptr %argc, align 4
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %3)
  %5 = load ptr, ptr %argv, align 8
  %6 = getelementptr inbounds ptr, ptr %5, i32 0
  %7 = load ptr, ptr %6, align 8
  %8 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr %7)
  %9 = load i32, ptr %argc, align 4
  %10 = icmp sgt i32 %9, 1
  br i1 %10, label %if.then.L4, label %if.exit.L4

if.then.L4:                                       ; preds = %2
  %11 = load ptr, ptr %argv, align 8
  %12 = getelementptr inbounds ptr, ptr %11, i32 1
  %13 = load ptr, ptr %12, align 8
  %14 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, ptr %13)
  br label %if.exit.L4

if.exit.L4:                                       ; preds = %if.then.L4, %2
  %15 = load i32, ptr %result, align 4
  ret i32 %15
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
