; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [3 x i8] c"%d\00", align 4
@printf.str.1 = private unnamed_addr constant [2 x i8] c".\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %q = alloca i64, align 8
  %q_new = alloca i64, align 8
  %r = alloca i64, align 8
  %r_new = alloca i64, align 8
  %t = alloca i64, align 8
  %t_new = alloca i64, align 8
  %k = alloca i64, align 8
  %k_new = alloca i64, align 8
  %m = alloca i64, align 8
  %x = alloca i64, align 8
  %iterations = alloca i32, align 4
  %printedDigits = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i64 1, ptr %q, align 8
  store i64 0, ptr %q_new, align 8
  store i64 0, ptr %r, align 8
  store i64 0, ptr %r_new, align 8
  store i64 1, ptr %t, align 8
  store i64 0, ptr %t_new, align 8
  store i64 1, ptr %k, align 8
  store i64 0, ptr %k_new, align 8
  store i64 3, ptr %m, align 8
  store i64 3, ptr %x, align 8
  store i32 20, ptr %iterations, align 4
  store i32 0, ptr %printedDigits, align 4
  store i32 0, ptr %i, align 4
  br label %for.head.L19

for.head.L19:                                     ; preds = %for.tail.L19, %0
  %1 = load i32, ptr %iterations, align 4
  %2 = load i32, ptr %i, align 4
  %3 = icmp slt i32 %2, %1
  br i1 %3, label %for.body.L19, label %for.exit.L19

for.body.L19:                                     ; preds = %for.head.L19
  %4 = load i64, ptr %q, align 8
  %5 = mul nsw i64 4, %4
  %6 = load i64, ptr %r, align 8
  %7 = add nsw i64 %5, %6
  %8 = load i64, ptr %t, align 8
  %9 = sub nsw i64 %7, %8
  %10 = load i64, ptr %t, align 8
  %11 = load i64, ptr %m, align 8
  %12 = mul nsw i64 %11, %10
  %13 = icmp slt i64 %9, %12
  br i1 %13, label %if.then.L20, label %if.else.L20

if.then.L20:                                      ; preds = %for.body.L19
  %14 = load i64, ptr %m, align 8
  %15 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i64 noundef %14)
  %16 = load i32, ptr %printedDigits, align 4
  %17 = icmp eq i32 %16, 0
  br i1 %17, label %if.then.L22, label %if.exit.L22

if.then.L22:                                      ; preds = %if.then.L20
  %18 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1)
  br label %if.exit.L22

if.exit.L22:                                      ; preds = %if.then.L22, %if.then.L20
  %19 = load i32, ptr %printedDigits, align 4
  %20 = add nsw i32 %19, 1
  store i32 %20, ptr %printedDigits, align 4
  %21 = load i64, ptr %q, align 8
  %22 = mul nsw i64 10, %21
  store i64 %22, ptr %q_new, align 8
  %23 = load i64, ptr %t, align 8
  %24 = load i64, ptr %m, align 8
  %25 = mul nsw i64 %24, %23
  %26 = load i64, ptr %r, align 8
  %27 = sub nsw i64 %26, %25
  %28 = mul nsw i64 10, %27
  store i64 %28, ptr %r_new, align 8
  %29 = load i64, ptr %q, align 8
  %30 = mul nsw i64 3, %29
  %31 = load i64, ptr %r, align 8
  %32 = add nsw i64 %30, %31
  %33 = mul nsw i64 10, %32
  %34 = load i64, ptr %t, align 8
  %35 = sdiv i64 %33, %34
  %36 = load i64, ptr %m, align 8
  %37 = mul nsw i64 10, %36
  %38 = sub nsw i64 %35, %37
  store i64 %38, ptr %m, align 8
  %39 = load i64, ptr %q_new, align 8
  store i64 %39, ptr %q, align 8
  %40 = load i64, ptr %r_new, align 8
  store i64 %40, ptr %r, align 8
  br label %if.exit.L20

if.else.L20:                                      ; preds = %for.body.L19
  %41 = load i64, ptr %k, align 8
  %42 = load i64, ptr %q, align 8
  %43 = mul nsw i64 %42, %41
  store i64 %43, ptr %q_new, align 8
  %44 = load i64, ptr %q, align 8
  %45 = mul nsw i64 2, %44
  %46 = load i64, ptr %r, align 8
  %47 = add nsw i64 %45, %46
  %48 = load i64, ptr %x, align 8
  %49 = mul nsw i64 %47, %48
  store i64 %49, ptr %r_new, align 8
  %50 = load i64, ptr %x, align 8
  %51 = load i64, ptr %t, align 8
  %52 = mul nsw i64 %51, %50
  store i64 %52, ptr %t_new, align 8
  %53 = load i64, ptr %k, align 8
  %54 = add nsw i64 %53, 1
  store i64 %54, ptr %k_new, align 8
  %55 = load i64, ptr %k, align 8
  %56 = mul nsw i64 7, %55
  %57 = add nsw i64 %56, 2
  %58 = load i64, ptr %q, align 8
  %59 = mul nsw i64 %58, %57
  %60 = load i64, ptr %x, align 8
  %61 = load i64, ptr %r, align 8
  %62 = mul nsw i64 %61, %60
  %63 = add nsw i64 %59, %62
  %64 = load i64, ptr %x, align 8
  %65 = load i64, ptr %t, align 8
  %66 = mul nsw i64 %65, %64
  %67 = sdiv i64 %63, %66
  store i64 %67, ptr %m, align 8
  %68 = load i64, ptr %x, align 8
  %69 = add nsw i64 %68, 2
  store i64 %69, ptr %x, align 8
  %70 = load i64, ptr %q_new, align 8
  store i64 %70, ptr %q, align 8
  %71 = load i64, ptr %r_new, align 8
  store i64 %71, ptr %r, align 8
  %72 = load i64, ptr %t_new, align 8
  store i64 %72, ptr %t, align 8
  %73 = load i64, ptr %k_new, align 8
  store i64 %73, ptr %k, align 8
  br label %if.exit.L20

if.exit.L20:                                      ; preds = %if.else.L20, %if.exit.L22
  br label %for.tail.L19

for.tail.L19:                                     ; preds = %if.exit.L20
  %74 = load i32, ptr %i, align 4
  %75 = add nsw i32 %74, 1
  store i32 %75, ptr %i, align 4
  br label %for.head.L19

for.exit.L19:                                     ; preds = %for.head.L19
  %76 = load i32, ptr %result, align 4
  ret i32 %76
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
