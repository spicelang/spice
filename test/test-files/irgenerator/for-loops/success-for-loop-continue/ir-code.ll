; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [13 x i8] c"Loop run %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [19 x i8] c"Inner loop run %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [5 x i8] c"End.\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %counter = alloca i32, align 4
  %subCounter = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 0, ptr %counter, align 4
  br label %for.head.L2

for.head.L2:                                      ; preds = %for.tail.L2, %0
  %1 = load i32, ptr %counter, align 4
  %2 = icmp slt i32 %1, 10
  br i1 %2, label %for.body.L2, label %for.exit.L2

for.body.L2:                                      ; preds = %for.head.L2
  %3 = load i32, ptr %counter, align 4
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %3)
  %5 = load i32, ptr %counter, align 4
  %6 = icmp sge i32 %5, 5
  br i1 %6, label %if.then.L4, label %if.exit.L4

if.then.L4:                                       ; preds = %for.body.L2
  store i32 100, ptr %subCounter, align 4
  br label %for.head.L5

for.head.L5:                                      ; preds = %for.tail.L5, %if.then.L4
  %7 = load i32, ptr %subCounter, align 4
  %8 = icmp sge i32 %7, 10
  br i1 %8, label %for.body.L5, label %for.exit.L5

for.body.L5:                                      ; preds = %for.head.L5
  %9 = load i32, ptr %subCounter, align 4
  %10 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %9)
  %11 = load i32, ptr %subCounter, align 4
  %12 = icmp eq i32 %11, 11
  br i1 %12, label %if.then.L7, label %if.exit.L7

if.then.L7:                                       ; preds = %for.body.L5
  br label %for.tail.L2

if.exit.L7:                                       ; preds = %for.body.L5
  br label %for.tail.L5

for.tail.L5:                                      ; preds = %if.exit.L7
  %13 = load i32, ptr %subCounter, align 4
  %14 = sub nsw i32 %13, 1
  store i32 %14, ptr %subCounter, align 4
  br label %for.head.L5

for.exit.L5:                                      ; preds = %for.head.L5
  br label %if.exit.L4

if.exit.L4:                                       ; preds = %for.exit.L5, %for.body.L2
  br label %for.tail.L2

for.tail.L2:                                      ; preds = %if.exit.L4, %if.then.L7
  %15 = load i32, ptr %counter, align 4
  %16 = add nsw i32 %15, 2
  store i32 %16, ptr %counter, align 4
  br label %for.head.L2

for.exit.L2:                                      ; preds = %for.head.L2
  %17 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2)
  %18 = load i32, ptr %result, align 4
  ret i32 %18
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
