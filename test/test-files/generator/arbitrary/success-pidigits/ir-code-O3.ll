; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1

; Function Attrs: nofree nounwind
define i32 @main() local_unnamed_addr #0 {
entry:
  br label %for

for:                                              ; preds = %entry, %for.inc
  %q.164 = phi i64 [ 1, %entry ], [ %q.0, %for.inc ]
  %r.163 = phi i64 [ 0, %entry ], [ %r.0, %for.inc ]
  %t.162 = phi i64 [ 1, %entry ], [ %t.0, %for.inc ]
  %k.161 = phi i64 [ 1, %entry ], [ %k.0, %for.inc ]
  %m.160 = phi i64 [ 3, %entry ], [ %m.0, %for.inc ]
  %x.159 = phi i64 [ 3, %entry ], [ %x.0, %for.inc ]
  %printedDigits.158 = phi i32 [ 0, %entry ], [ %printedDigits.0, %for.inc ]
  %i.057 = phi i32 [ 0, %entry ], [ %29, %for.inc ]
  %0 = shl i64 %q.164, 2
  %1 = sub i64 %r.163, %t.162
  %2 = add i64 %1, %0
  %3 = mul i64 %t.162, %m.160
  %4 = icmp slt i64 %2, %3
  br i1 %4, label %if.then, label %if.else

if.then:                                          ; preds = %for
  %5 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([3 x i8], [3 x i8]* @0, i64 0, i64 0), i64 %m.160)
  %6 = icmp eq i32 %printedDigits.158, 0
  br i1 %6, label %if.then1, label %if.end

if.then1:                                         ; preds = %if.then
  %putchar = tail call i32 @putchar(i32 46)
  br label %if.end

if.end:                                           ; preds = %if.then1, %if.then
  %7 = add i32 %printedDigits.158, 1
  %8 = mul i64 %q.164, 10
  %9 = sub i64 %r.163, %3
  %10 = mul i64 %9, 10
  %11 = mul i64 %q.164, 3
  %12 = add i64 %11, %r.163
  %13 = mul i64 %12, 10
  %14 = sdiv i64 %13, %t.162
  %.neg = mul i64 %m.160, -10
  %15 = add i64 %14, %.neg
  br label %for.inc

if.else:                                          ; preds = %for
  %16 = mul i64 %q.164, %k.161
  %17 = shl i64 %q.164, 1
  %18 = add i64 %17, %r.163
  %19 = mul i64 %18, %x.159
  %20 = mul i64 %t.162, %x.159
  %21 = add i64 %k.161, 1
  %22 = mul i64 %k.161, 7
  %23 = add i64 %22, 2
  %24 = mul i64 %q.164, %23
  %25 = mul i64 %r.163, %x.159
  %26 = add i64 %24, %25
  %27 = sdiv i64 %26, %20
  %28 = add i64 %x.159, 2
  br label %for.inc

for.inc:                                          ; preds = %if.end, %if.else
  %printedDigits.0 = phi i32 [ %7, %if.end ], [ %printedDigits.158, %if.else ]
  %x.0 = phi i64 [ %x.159, %if.end ], [ %28, %if.else ]
  %m.0 = phi i64 [ %15, %if.end ], [ %27, %if.else ]
  %k.0 = phi i64 [ %k.161, %if.end ], [ %21, %if.else ]
  %t.0 = phi i64 [ %t.162, %if.end ], [ %20, %if.else ]
  %r.0 = phi i64 [ %10, %if.end ], [ %19, %if.else ]
  %q.0 = phi i64 [ %8, %if.end ], [ %16, %if.else ]
  %29 = add nuw nsw i32 %i.057, 1
  %exitcond.not = icmp eq i32 %29, 20
  br i1 %exitcond.not, label %for.end, label %for

for.end:                                          ; preds = %for.inc
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(i8* nocapture noundef readonly, ...) local_unnamed_addr #0

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
