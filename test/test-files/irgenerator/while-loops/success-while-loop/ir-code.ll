; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [17 x i8] c"i is now at: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 0, ptr %i, align 4
  br label %while.head.L3

while.head.L3:                                    ; preds = %while.body.L3, %0
  %1 = load i32, ptr %i, align 4
  %2 = icmp slt i32 %1, 10
  br i1 %2, label %while.body.L3, label %while.exit.L3

while.body.L3:                                    ; preds = %while.head.L3
  %3 = load i32, ptr %i, align 4
  %4 = add nsw i32 %3, 1
  store i32 %4, ptr %i, align 4
  %5 = load i32, ptr %i, align 4
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %5)
  br label %while.head.L3

while.exit.L3:                                    ; preds = %while.head.L3
  %7 = load i32, ptr %result, align 4
  ret i32 %7
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
