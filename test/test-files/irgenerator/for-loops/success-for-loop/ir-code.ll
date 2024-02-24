; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [9 x i8] c"Step %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 0, ptr %i, align 4
  br label %for.head.L2

for.head.L2:                                      ; preds = %for.tail.L2, %0
  %1 = load i32, ptr %i, align 4
  %2 = icmp slt i32 %1, 10
  br i1 %2, label %for.body.L2, label %for.exit.L2

for.body.L2:                                      ; preds = %for.head.L2
  %3 = load i32, ptr %i, align 4
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %3)
  br label %for.tail.L2

for.tail.L2:                                      ; preds = %for.body.L2
  %5 = load i32, ptr %i, align 4
  %6 = add nsw i32 %5, 2
  store i32 %6, ptr %i, align 4
  br label %for.head.L2

for.exit.L2:                                      ; preds = %for.head.L2
  %7 = load i32, ptr %result, align 4
  ret i32 %7
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
