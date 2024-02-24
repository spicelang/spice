; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [36 x i8] c"Ackermann of base m=%d and n=%d: %d\00", align 1

define private i32 @_Z3ackii(i32 %0, i32 %1) {
  %result = alloca i32, align 4
  %m = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 %0, ptr %m, align 4
  store i32 %1, ptr %n, align 4
  %3 = load i32, ptr %m, align 4
  %4 = icmp eq i32 %3, 0
  br i1 %4, label %if.then.L2, label %if.exit.L2

if.then.L2:                                       ; preds = %2
  %5 = load i32, ptr %n, align 4
  %6 = add nsw i32 %5, 1
  ret i32 %6

if.exit.L2:                                       ; preds = %2
  %7 = load i32, ptr %n, align 4
  %8 = icmp eq i32 %7, 0
  br i1 %8, label %if.then.L3, label %if.exit.L3

if.then.L3:                                       ; preds = %if.exit.L2
  %9 = load i32, ptr %m, align 4
  %10 = sub nsw i32 %9, 1
  %11 = call i32 @_Z3ackii(i32 %10, i32 1)
  ret i32 %11

if.exit.L3:                                       ; preds = %if.exit.L2
  %12 = load i32, ptr %m, align 4
  %13 = sub nsw i32 %12, 1
  %14 = load i32, ptr %m, align 4
  %15 = load i32, ptr %n, align 4
  %16 = sub nsw i32 %15, 1
  %17 = call i32 @_Z3ackii(i32 %14, i32 %16)
  %18 = call i32 @_Z3ackii(i32 %13, i32 %17)
  ret i32 %18
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %baseM = alloca i32, align 4
  %baseN = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 3, ptr %baseM, align 4
  store i32 10, ptr %baseN, align 4
  %1 = load i32, ptr %baseM, align 4
  %2 = load i32, ptr %baseN, align 4
  %3 = load i32, ptr %baseM, align 4
  %4 = load i32, ptr %baseN, align 4
  %5 = call i32 @_Z3ackii(i32 %3, i32 %4)
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %1, i32 %2, i32 %5)
  %7 = load i32, ptr %result, align 4
  ret i32 %7
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
