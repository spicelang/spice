; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [6 x i8] c"Pizza\00", align 1
@printf.str.0 = private unnamed_addr constant [32 x i8] c"Pointer address: %p, value: %s\0A\00", align 1
@anon.string.1 = private unnamed_addr constant [7 x i8] c"Burger\00", align 1
@printf.str.1 = private unnamed_addr constant [20 x i8] c"Restored value: %s\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [27 x i8] c"Restored value address: %p\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %food = alloca ptr, align 8
  %ptr = alloca ptr, align 8
  %restoredFood = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  store ptr @anon.string.0, ptr %food, align 8
  store ptr %food, ptr %ptr, align 8
  %1 = load ptr, ptr %ptr, align 8
  %2 = load ptr, ptr %ptr, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr %1, ptr %3)
  %5 = load ptr, ptr %ptr, align 8
  store ptr @anon.string.1, ptr %5, align 8
  %6 = load ptr, ptr %ptr, align 8
  %7 = load ptr, ptr %6, align 8
  store ptr %7, ptr %restoredFood, align 8
  %8 = load ptr, ptr %restoredFood, align 8
  %9 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr %8)
  %10 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, ptr %restoredFood)
  %11 = load i32, ptr %result, align 4
  ret i32 %11
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
