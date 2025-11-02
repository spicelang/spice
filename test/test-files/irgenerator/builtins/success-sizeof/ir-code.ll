; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [20 x i8] c"Size of double: %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [17 x i8] c"Size of int: %d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [19 x i8] c"Size of short: %d\0A\00", align 4
@printf.str.3 = private unnamed_addr constant [18 x i8] c"Size of long: %d\0A\00", align 4
@printf.str.4 = private unnamed_addr constant [18 x i8] c"Size of byte: %d\0A\00", align 4
@printf.str.5 = private unnamed_addr constant [18 x i8] c"Size of char: %d\0A\00", align 4
@printf.str.6 = private unnamed_addr constant [20 x i8] c"Size of string: %d\0A\00", align 4
@printf.str.7 = private unnamed_addr constant [18 x i8] c"Size of bool: %d\0A\00", align 4
@printf.str.8 = private unnamed_addr constant [20 x i8] c"Size of int[7]: %d\0A\00", align 4
@printf.str.9 = private unnamed_addr constant [18 x i8] c"Size of int*: %d\0A\00", align 4
@printf.str.10 = private unnamed_addr constant [25 x i8] c"Size of struct type: %d\0A\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %intVariable = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i64 noundef 8)
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i64 noundef 4)
  %3 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i64 noundef 2)
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i64 noundef 8)
  %5 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i64 noundef 1)
  %6 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.5, i64 noundef 1)
  %7 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.6, i64 noundef 8)
  %8 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.7, i64 noundef 1)
  %9 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.8, i64 noundef 28)
  store i32 123, ptr %intVariable, align 4
  %10 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.9, i64 noundef 8)
  %11 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.10, i64 noundef 16)
  %12 = load i32, ptr %result, align 4
  ret i32 %12
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
