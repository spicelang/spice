; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [15 x i8] c"Cell [1,3]: %d\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %a = alloca [10 x [10 x i32]], align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store [10 x [10 x i32]] zeroinitializer, ptr %a, align 4
  store i32 0, ptr %i, align 4
  br label %for.head.L3

for.head.L3:                                      ; preds = %for.tail.L3, %0
  %1 = load i32, ptr %i, align 4
  %2 = icmp slt i32 %1, 10
  br i1 %2, label %for.body.L3, label %for.exit.L3

for.body.L3:                                      ; preds = %for.head.L3
  store i32 0, ptr %j, align 4
  br label %for.head.L4

for.head.L4:                                      ; preds = %for.tail.L4, %for.body.L3
  %3 = load i32, ptr %j, align 4
  %4 = icmp slt i32 %3, 10
  br i1 %4, label %for.body.L4, label %for.exit.L4

for.body.L4:                                      ; preds = %for.head.L4
  %5 = load i32, ptr %i, align 4
  %6 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i32 %5
  %7 = load i32, ptr %j, align 4
  %8 = getelementptr inbounds [10 x i32], ptr %6, i64 0, i32 %7
  %9 = load i32, ptr %j, align 4
  %10 = load i32, ptr %i, align 4
  %11 = mul nsw i32 %10, %9
  store i32 %11, ptr %8, align 4
  br label %for.tail.L4

for.tail.L4:                                      ; preds = %for.body.L4
  %12 = load i32, ptr %j, align 4
  %13 = add nsw i32 %12, 1
  store i32 %13, ptr %j, align 4
  br label %for.head.L4

for.exit.L4:                                      ; preds = %for.head.L4
  br label %for.tail.L3

for.tail.L3:                                      ; preds = %for.exit.L4
  %14 = load i32, ptr %i, align 4
  %15 = add nsw i32 %14, 1
  store i32 %15, ptr %i, align 4
  br label %for.head.L3

for.exit.L3:                                      ; preds = %for.head.L3
  %16 = getelementptr inbounds [10 x [10 x i32]], ptr %a, i64 0, i32 1
  %17 = getelementptr inbounds [10 x i32], ptr %16, i64 0, i32 3
  %18 = load i32, ptr %17, align 4
  %19 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %18)
  %20 = load i32, ptr %result, align 4
  ret i32 %20
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
