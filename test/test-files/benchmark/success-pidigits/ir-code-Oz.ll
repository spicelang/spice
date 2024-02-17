; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  br label %for.body.L19

for.body.L19:                                     ; preds = %0, %for.tail.L19
  %i.035 = phi i32 [ 0, %0 ], [ %30, %for.tail.L19 ]
  %printedDigits.034 = phi i32 [ 0, %0 ], [ %printedDigits.1, %for.tail.L19 ]
  %q.033 = phi i64 [ 1, %0 ], [ %q.1, %for.tail.L19 ]
  %x.032 = phi i64 [ 3, %0 ], [ %x.1, %for.tail.L19 ]
  %m.031 = phi i64 [ 3, %0 ], [ %m.1, %for.tail.L19 ]
  %k.030 = phi i64 [ 1, %0 ], [ %k.1, %for.tail.L19 ]
  %t.029 = phi i64 [ 1, %0 ], [ %t.1, %for.tail.L19 ]
  %r.028 = phi i64 [ 0, %0 ], [ %r.1, %for.tail.L19 ]
  %1 = shl nsw i64 %q.033, 2
  %2 = sub i64 %1, %t.029
  %3 = add i64 %2, %r.028
  %4 = mul nsw i64 %t.029, %m.031
  %5 = icmp slt i64 %3, %4
  br i1 %5, label %if.then.L20, label %if.else.L20

if.then.L20:                                      ; preds = %for.body.L19
  %6 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i64 %m.031)
  %7 = icmp eq i32 %printedDigits.034, 0
  br i1 %7, label %if.then.L22, label %if.exit.L22

if.then.L22:                                      ; preds = %if.then.L20
  %putchar = tail call i32 @putchar(i32 46)
  br label %if.exit.L22

if.exit.L22:                                      ; preds = %if.then.L22, %if.then.L20
  %8 = add nsw i32 %printedDigits.034, 1
  %9 = mul nsw i64 %q.033, 10
  %10 = sub nsw i64 %r.028, %4
  %11 = mul nsw i64 %10, 10
  %12 = mul nsw i64 %q.033, 3
  %13 = add nsw i64 %r.028, %12
  %14 = mul nsw i64 %13, 10
  %15 = sdiv i64 %14, %t.029
  %.neg = mul i64 %m.031, -10
  %16 = add i64 %15, %.neg
  br label %for.tail.L19

if.else.L20:                                      ; preds = %for.body.L19
  %17 = mul nsw i64 %k.030, %q.033
  %18 = shl nsw i64 %q.033, 1
  %19 = add nsw i64 %r.028, %18
  %20 = mul nsw i64 %19, %x.032
  %21 = mul nsw i64 %t.029, %x.032
  %22 = add nsw i64 %k.030, 1
  %23 = mul nsw i64 %k.030, 7
  %24 = add nsw i64 %23, 2
  %25 = mul nsw i64 %24, %q.033
  %26 = mul nsw i64 %r.028, %x.032
  %27 = add nsw i64 %25, %26
  %28 = sdiv i64 %27, %21
  %29 = add nsw i64 %x.032, 2
  br label %for.tail.L19

for.tail.L19:                                     ; preds = %if.exit.L22, %if.else.L20
  %r.1 = phi i64 [ %11, %if.exit.L22 ], [ %20, %if.else.L20 ]
  %t.1 = phi i64 [ %t.029, %if.exit.L22 ], [ %21, %if.else.L20 ]
  %k.1 = phi i64 [ %k.030, %if.exit.L22 ], [ %22, %if.else.L20 ]
  %m.1 = phi i64 [ %16, %if.exit.L22 ], [ %28, %if.else.L20 ]
  %x.1 = phi i64 [ %x.032, %if.exit.L22 ], [ %29, %if.else.L20 ]
  %q.1 = phi i64 [ %9, %if.exit.L22 ], [ %17, %if.else.L20 ]
  %printedDigits.1 = phi i32 [ %8, %if.exit.L22 ], [ %printedDigits.034, %if.else.L20 ]
  %30 = add nuw nsw i32 %i.035, 1
  %exitcond.not = icmp eq i32 %30, 20
  br i1 %exitcond.not, label %for.exit.L19, label %for.body.L19

for.exit.L19:                                     ; preds = %for.tail.L19
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) local_unnamed_addr #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
