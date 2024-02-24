; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [11 x i8] c"Result: %d\00", align 1

define private i32 @_Z4fiboi(i32 %0) {
  %result = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 %0, ptr %n, align 4
  %2 = load i32, ptr %n, align 4
  %3 = icmp sle i32 %2, 1
  br i1 %3, label %if.then.L2, label %if.exit.L2

if.then.L2:                                       ; preds = %1
  %4 = load i32, ptr %n, align 4
  ret i32 %4

if.exit.L2:                                       ; preds = %1
  %5 = load i32, ptr %n, align 4
  %6 = sub nsw i32 %5, 1
  %7 = call i32 @_Z4fiboi(i32 %6)
  %8 = load i32, ptr %n, align 4
  %9 = sub nsw i32 %8, 2
  %10 = call i32 @_Z4fiboi(i32 %9)
  %11 = add nsw i32 %7, %10
  ret i32 %11
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call i32 @_Z4fiboi(i32 30)
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %1)
  %3 = load i32, ptr %result, align 4
  ret i32 %3
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
