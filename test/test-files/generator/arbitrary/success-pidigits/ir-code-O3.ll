; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1

; Function Attrs: nofree nounwind
define i32 @main() local_unnamed_addr #0 {
entry:
  br label %for

for:                                              ; preds = %entry, %for.post
  %q.064 = phi i64 [ 1, %entry ], [ %q.1, %for.post ]
  %r.063 = phi i64 [ 0, %entry ], [ %r.1, %for.post ]
  %t.062 = phi i64 [ 1, %entry ], [ %t.1, %for.post ]
  %k.061 = phi i64 [ 1, %entry ], [ %k.1, %for.post ]
  %m.060 = phi i64 [ 3, %entry ], [ %m.1, %for.post ]
  %x.059 = phi i64 [ 3, %entry ], [ %x.1, %for.post ]
  %printedDigits.058 = phi i32 [ 0, %entry ], [ %printedDigits.1, %for.post ]
  %i.057 = phi i32 [ 0, %entry ], [ %29, %for.post ]
  %0 = shl i64 %q.064, 2
  %1 = sub i64 %r.063, %t.062
  %2 = add i64 %1, %0
  %3 = mul i64 %t.062, %m.060
  %4 = icmp slt i64 %2, %3
  br i1 %4, label %if.then, label %if.else

if.then:                                          ; preds = %for
  %5 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([3 x i8], [3 x i8]* @0, i64 0, i64 0), i64 %m.060)
  %6 = icmp eq i32 %printedDigits.058, 0
  br i1 %6, label %if.then1, label %if.end

if.then1:                                         ; preds = %if.then
  %putchar = tail call i32 @putchar(i32 46)
  br label %if.end

if.end:                                           ; preds = %if.then1, %if.then
  %7 = add i32 %printedDigits.058, 1
  %8 = mul i64 %q.064, 10
  %9 = sub i64 %r.063, %3
  %10 = mul i64 %9, 10
  %11 = mul i64 %q.064, 3
  %12 = add i64 %11, %r.063
  %13 = mul i64 %12, 10
  %14 = sdiv i64 %13, %t.062
  %.neg = mul i64 %m.060, -10
  %15 = add i64 %14, %.neg
  br label %for.post

if.else:                                          ; preds = %for
  %16 = mul i64 %q.064, %k.061
  %17 = shl i64 %q.064, 1
  %18 = add i64 %17, %r.063
  %19 = mul i64 %18, %x.059
  %20 = mul i64 %t.062, %x.059
  %21 = add i64 %k.061, 1
  %22 = mul i64 %k.061, 7
  %23 = add i64 %22, 2
  %24 = mul i64 %q.064, %23
  %25 = mul i64 %r.063, %x.059
  %26 = add i64 %24, %25
  %27 = sdiv i64 %26, %20
  %28 = add i64 %x.059, 2
  br label %for.post

for.post:                                         ; preds = %if.end, %if.else
  %printedDigits.1 = phi i32 [ %7, %if.end ], [ %printedDigits.058, %if.else ]
  %x.1 = phi i64 [ %x.059, %if.end ], [ %28, %if.else ]
  %m.1 = phi i64 [ %15, %if.end ], [ %27, %if.else ]
  %k.1 = phi i64 [ %k.061, %if.end ], [ %21, %if.else ]
  %t.1 = phi i64 [ %t.062, %if.end ], [ %20, %if.else ]
  %r.1 = phi i64 [ %10, %if.end ], [ %19, %if.else ]
  %q.1 = phi i64 [ %8, %if.end ], [ %16, %if.else ]
  %29 = add nuw nsw i32 %i.057, 1
  %exitcond.not = icmp eq i32 %29, 20
  br i1 %exitcond.not, label %for.end, label %for

for.end:                                          ; preds = %for.post
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(i8* nocapture noundef readonly, ...) local_unnamed_addr #0

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
