; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@anon.string.0 = private unnamed_addr constant [2 x i8] c"*\00", align 1
@anon.string.1 = private unnamed_addr constant [2 x i8] c" \00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
for.exit.L8:
  %board = alloca [80 x i32], align 4
  %.fca.1.gep = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 1
  %.fca.78.gep = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 78
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 4 dereferenceable(320) %board, i8 0, i64 320, i1 false)
  store i32 1, ptr %.fca.78.gep, align 4
  br label %for.head.L15.preheader

for.head.L15.preheader:                           ; preds = %for.exit.L8, %for.tail.L14
  %i1.016 = phi i32 [ 0, %for.exit.L8 ], [ %318, %for.tail.L14 ]
  br label %for.body.L15

for.body.L15:                                     ; preds = %for.head.L15.preheader, %for.body.L15
  %indvars.iv = phi i64 [ 0, %for.head.L15.preheader ], [ %indvars.iv.next, %for.body.L15 ]
  %0 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv
  %1 = load i32, ptr %0, align 4
  %2 = icmp eq i32 %1, 1
  %3 = select i1 %2, ptr @anon.string.0, ptr @anon.string.1
  %4 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, ptr nonnull %3)
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 80
  br i1 %exitcond.not, label %for.exit.L15, label %for.body.L15

for.exit.L15:                                     ; preds = %for.body.L15
  %putchar = tail call i32 @putchar(i32 10)
  %5 = load i32, ptr %.fca.1.gep, align 4
  br label %for.body.L21

for.body.L21:                                     ; preds = %for.body.L21, %for.exit.L15
  %indvars.iv19 = phi i64 [ 1, %for.exit.L15 ], [ %indvars.iv.next20.38, %for.body.L21 ]
  %pattern.014 = phi i32 [ %5, %for.exit.L15 ], [ %314, %for.body.L21 ]
  %6 = shl i32 %pattern.014, 1
  %7 = and i32 %6, 6
  %indvars.iv.next20 = add nuw nsw i64 %indvars.iv19, 1
  %8 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20
  %9 = load i32, ptr %8, align 4
  %10 = or i32 %9, %7
  %11 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv19
  %12 = lshr i32 110, %10
  %13 = and i32 %12, 1
  store i32 %13, ptr %11, align 4
  %14 = shl i32 %10, 1
  %15 = and i32 %14, 6
  %indvars.iv.next20.1 = add nuw nsw i64 %indvars.iv19, 2
  %16 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.1
  %17 = load i32, ptr %16, align 4
  %18 = or i32 %17, %15
  %19 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20
  %20 = lshr i32 110, %18
  %21 = and i32 %20, 1
  store i32 %21, ptr %19, align 4
  %22 = shl i32 %18, 1
  %23 = and i32 %22, 6
  %indvars.iv.next20.2 = add nuw nsw i64 %indvars.iv19, 3
  %24 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.2
  %25 = load i32, ptr %24, align 4
  %26 = or i32 %25, %23
  %27 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.1
  %28 = lshr i32 110, %26
  %29 = and i32 %28, 1
  store i32 %29, ptr %27, align 4
  %30 = shl i32 %26, 1
  %31 = and i32 %30, 6
  %indvars.iv.next20.3 = add nuw nsw i64 %indvars.iv19, 4
  %32 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.3
  %33 = load i32, ptr %32, align 4
  %34 = or i32 %33, %31
  %35 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.2
  %36 = lshr i32 110, %34
  %37 = and i32 %36, 1
  store i32 %37, ptr %35, align 4
  %38 = shl i32 %34, 1
  %39 = and i32 %38, 6
  %indvars.iv.next20.4 = add nuw nsw i64 %indvars.iv19, 5
  %40 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.4
  %41 = load i32, ptr %40, align 4
  %42 = or i32 %41, %39
  %43 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.3
  %44 = lshr i32 110, %42
  %45 = and i32 %44, 1
  store i32 %45, ptr %43, align 4
  %46 = shl i32 %42, 1
  %47 = and i32 %46, 6
  %indvars.iv.next20.5 = add nuw nsw i64 %indvars.iv19, 6
  %48 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.5
  %49 = load i32, ptr %48, align 4
  %50 = or i32 %49, %47
  %51 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.4
  %52 = lshr i32 110, %50
  %53 = and i32 %52, 1
  store i32 %53, ptr %51, align 4
  %54 = shl i32 %50, 1
  %55 = and i32 %54, 6
  %indvars.iv.next20.6 = add nuw nsw i64 %indvars.iv19, 7
  %56 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.6
  %57 = load i32, ptr %56, align 4
  %58 = or i32 %57, %55
  %59 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.5
  %60 = lshr i32 110, %58
  %61 = and i32 %60, 1
  store i32 %61, ptr %59, align 4
  %62 = shl i32 %58, 1
  %63 = and i32 %62, 6
  %indvars.iv.next20.7 = add nuw nsw i64 %indvars.iv19, 8
  %64 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.7
  %65 = load i32, ptr %64, align 4
  %66 = or i32 %65, %63
  %67 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.6
  %68 = lshr i32 110, %66
  %69 = and i32 %68, 1
  store i32 %69, ptr %67, align 4
  %70 = shl i32 %66, 1
  %71 = and i32 %70, 6
  %indvars.iv.next20.8 = add nuw nsw i64 %indvars.iv19, 9
  %72 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.8
  %73 = load i32, ptr %72, align 4
  %74 = or i32 %73, %71
  %75 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.7
  %76 = lshr i32 110, %74
  %77 = and i32 %76, 1
  store i32 %77, ptr %75, align 4
  %78 = shl i32 %74, 1
  %79 = and i32 %78, 6
  %indvars.iv.next20.9 = add nuw nsw i64 %indvars.iv19, 10
  %80 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.9
  %81 = load i32, ptr %80, align 4
  %82 = or i32 %81, %79
  %83 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.8
  %84 = lshr i32 110, %82
  %85 = and i32 %84, 1
  store i32 %85, ptr %83, align 4
  %86 = shl i32 %82, 1
  %87 = and i32 %86, 6
  %indvars.iv.next20.10 = add nuw nsw i64 %indvars.iv19, 11
  %88 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.10
  %89 = load i32, ptr %88, align 4
  %90 = or i32 %89, %87
  %91 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.9
  %92 = lshr i32 110, %90
  %93 = and i32 %92, 1
  store i32 %93, ptr %91, align 4
  %94 = shl i32 %90, 1
  %95 = and i32 %94, 6
  %indvars.iv.next20.11 = add nuw nsw i64 %indvars.iv19, 12
  %96 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.11
  %97 = load i32, ptr %96, align 4
  %98 = or i32 %97, %95
  %99 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.10
  %100 = lshr i32 110, %98
  %101 = and i32 %100, 1
  store i32 %101, ptr %99, align 4
  %102 = shl i32 %98, 1
  %103 = and i32 %102, 6
  %indvars.iv.next20.12 = add nuw nsw i64 %indvars.iv19, 13
  %104 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.12
  %105 = load i32, ptr %104, align 4
  %106 = or i32 %105, %103
  %107 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.11
  %108 = lshr i32 110, %106
  %109 = and i32 %108, 1
  store i32 %109, ptr %107, align 4
  %110 = shl i32 %106, 1
  %111 = and i32 %110, 6
  %indvars.iv.next20.13 = add nuw nsw i64 %indvars.iv19, 14
  %112 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.13
  %113 = load i32, ptr %112, align 4
  %114 = or i32 %113, %111
  %115 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.12
  %116 = lshr i32 110, %114
  %117 = and i32 %116, 1
  store i32 %117, ptr %115, align 4
  %118 = shl i32 %114, 1
  %119 = and i32 %118, 6
  %indvars.iv.next20.14 = add nuw nsw i64 %indvars.iv19, 15
  %120 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.14
  %121 = load i32, ptr %120, align 4
  %122 = or i32 %121, %119
  %123 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.13
  %124 = lshr i32 110, %122
  %125 = and i32 %124, 1
  store i32 %125, ptr %123, align 4
  %126 = shl i32 %122, 1
  %127 = and i32 %126, 6
  %indvars.iv.next20.15 = add nuw nsw i64 %indvars.iv19, 16
  %128 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.15
  %129 = load i32, ptr %128, align 4
  %130 = or i32 %129, %127
  %131 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.14
  %132 = lshr i32 110, %130
  %133 = and i32 %132, 1
  store i32 %133, ptr %131, align 4
  %134 = shl i32 %130, 1
  %135 = and i32 %134, 6
  %indvars.iv.next20.16 = add nuw nsw i64 %indvars.iv19, 17
  %136 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.16
  %137 = load i32, ptr %136, align 4
  %138 = or i32 %137, %135
  %139 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.15
  %140 = lshr i32 110, %138
  %141 = and i32 %140, 1
  store i32 %141, ptr %139, align 4
  %142 = shl i32 %138, 1
  %143 = and i32 %142, 6
  %indvars.iv.next20.17 = add nuw nsw i64 %indvars.iv19, 18
  %144 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.17
  %145 = load i32, ptr %144, align 4
  %146 = or i32 %145, %143
  %147 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.16
  %148 = lshr i32 110, %146
  %149 = and i32 %148, 1
  store i32 %149, ptr %147, align 4
  %150 = shl i32 %146, 1
  %151 = and i32 %150, 6
  %indvars.iv.next20.18 = add nuw nsw i64 %indvars.iv19, 19
  %152 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.18
  %153 = load i32, ptr %152, align 4
  %154 = or i32 %153, %151
  %155 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.17
  %156 = lshr i32 110, %154
  %157 = and i32 %156, 1
  store i32 %157, ptr %155, align 4
  %158 = shl i32 %154, 1
  %159 = and i32 %158, 6
  %indvars.iv.next20.19 = add nuw nsw i64 %indvars.iv19, 20
  %160 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.19
  %161 = load i32, ptr %160, align 4
  %162 = or i32 %161, %159
  %163 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.18
  %164 = lshr i32 110, %162
  %165 = and i32 %164, 1
  store i32 %165, ptr %163, align 4
  %166 = shl i32 %162, 1
  %167 = and i32 %166, 6
  %indvars.iv.next20.20 = add nuw nsw i64 %indvars.iv19, 21
  %168 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.20
  %169 = load i32, ptr %168, align 4
  %170 = or i32 %169, %167
  %171 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.19
  %172 = lshr i32 110, %170
  %173 = and i32 %172, 1
  store i32 %173, ptr %171, align 4
  %174 = shl i32 %170, 1
  %175 = and i32 %174, 6
  %indvars.iv.next20.21 = add nuw nsw i64 %indvars.iv19, 22
  %176 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.21
  %177 = load i32, ptr %176, align 4
  %178 = or i32 %177, %175
  %179 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.20
  %180 = lshr i32 110, %178
  %181 = and i32 %180, 1
  store i32 %181, ptr %179, align 4
  %182 = shl i32 %178, 1
  %183 = and i32 %182, 6
  %indvars.iv.next20.22 = add nuw nsw i64 %indvars.iv19, 23
  %184 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.22
  %185 = load i32, ptr %184, align 4
  %186 = or i32 %185, %183
  %187 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.21
  %188 = lshr i32 110, %186
  %189 = and i32 %188, 1
  store i32 %189, ptr %187, align 4
  %190 = shl i32 %186, 1
  %191 = and i32 %190, 6
  %indvars.iv.next20.23 = add nuw nsw i64 %indvars.iv19, 24
  %192 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.23
  %193 = load i32, ptr %192, align 4
  %194 = or i32 %193, %191
  %195 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.22
  %196 = lshr i32 110, %194
  %197 = and i32 %196, 1
  store i32 %197, ptr %195, align 4
  %198 = shl i32 %194, 1
  %199 = and i32 %198, 6
  %indvars.iv.next20.24 = add nuw nsw i64 %indvars.iv19, 25
  %200 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.24
  %201 = load i32, ptr %200, align 4
  %202 = or i32 %201, %199
  %203 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.23
  %204 = lshr i32 110, %202
  %205 = and i32 %204, 1
  store i32 %205, ptr %203, align 4
  %206 = shl i32 %202, 1
  %207 = and i32 %206, 6
  %indvars.iv.next20.25 = add nuw nsw i64 %indvars.iv19, 26
  %208 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.25
  %209 = load i32, ptr %208, align 4
  %210 = or i32 %209, %207
  %211 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.24
  %212 = lshr i32 110, %210
  %213 = and i32 %212, 1
  store i32 %213, ptr %211, align 4
  %214 = shl i32 %210, 1
  %215 = and i32 %214, 6
  %indvars.iv.next20.26 = add nuw nsw i64 %indvars.iv19, 27
  %216 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.26
  %217 = load i32, ptr %216, align 4
  %218 = or i32 %217, %215
  %219 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.25
  %220 = lshr i32 110, %218
  %221 = and i32 %220, 1
  store i32 %221, ptr %219, align 4
  %222 = shl i32 %218, 1
  %223 = and i32 %222, 6
  %indvars.iv.next20.27 = add nuw nsw i64 %indvars.iv19, 28
  %224 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.27
  %225 = load i32, ptr %224, align 4
  %226 = or i32 %225, %223
  %227 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.26
  %228 = lshr i32 110, %226
  %229 = and i32 %228, 1
  store i32 %229, ptr %227, align 4
  %230 = shl i32 %226, 1
  %231 = and i32 %230, 6
  %indvars.iv.next20.28 = add nuw nsw i64 %indvars.iv19, 29
  %232 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.28
  %233 = load i32, ptr %232, align 4
  %234 = or i32 %233, %231
  %235 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.27
  %236 = lshr i32 110, %234
  %237 = and i32 %236, 1
  store i32 %237, ptr %235, align 4
  %238 = shl i32 %234, 1
  %239 = and i32 %238, 6
  %indvars.iv.next20.29 = add nuw nsw i64 %indvars.iv19, 30
  %240 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.29
  %241 = load i32, ptr %240, align 4
  %242 = or i32 %241, %239
  %243 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.28
  %244 = lshr i32 110, %242
  %245 = and i32 %244, 1
  store i32 %245, ptr %243, align 4
  %246 = shl i32 %242, 1
  %247 = and i32 %246, 6
  %indvars.iv.next20.30 = add nuw nsw i64 %indvars.iv19, 31
  %248 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.30
  %249 = load i32, ptr %248, align 4
  %250 = or i32 %249, %247
  %251 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.29
  %252 = lshr i32 110, %250
  %253 = and i32 %252, 1
  store i32 %253, ptr %251, align 4
  %254 = shl i32 %250, 1
  %255 = and i32 %254, 6
  %indvars.iv.next20.31 = add nuw nsw i64 %indvars.iv19, 32
  %256 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.31
  %257 = load i32, ptr %256, align 4
  %258 = or i32 %257, %255
  %259 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.30
  %260 = lshr i32 110, %258
  %261 = and i32 %260, 1
  store i32 %261, ptr %259, align 4
  %262 = shl i32 %258, 1
  %263 = and i32 %262, 6
  %indvars.iv.next20.32 = add nuw nsw i64 %indvars.iv19, 33
  %264 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.32
  %265 = load i32, ptr %264, align 4
  %266 = or i32 %265, %263
  %267 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.31
  %268 = lshr i32 110, %266
  %269 = and i32 %268, 1
  store i32 %269, ptr %267, align 4
  %270 = shl i32 %266, 1
  %271 = and i32 %270, 6
  %indvars.iv.next20.33 = add nuw nsw i64 %indvars.iv19, 34
  %272 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.33
  %273 = load i32, ptr %272, align 4
  %274 = or i32 %273, %271
  %275 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.32
  %276 = lshr i32 110, %274
  %277 = and i32 %276, 1
  store i32 %277, ptr %275, align 4
  %278 = shl i32 %274, 1
  %279 = and i32 %278, 6
  %indvars.iv.next20.34 = add nuw nsw i64 %indvars.iv19, 35
  %280 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.34
  %281 = load i32, ptr %280, align 4
  %282 = or i32 %281, %279
  %283 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.33
  %284 = lshr i32 110, %282
  %285 = and i32 %284, 1
  store i32 %285, ptr %283, align 4
  %286 = shl i32 %282, 1
  %287 = and i32 %286, 6
  %indvars.iv.next20.35 = add nuw nsw i64 %indvars.iv19, 36
  %288 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.35
  %289 = load i32, ptr %288, align 4
  %290 = or i32 %289, %287
  %291 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.34
  %292 = lshr i32 110, %290
  %293 = and i32 %292, 1
  store i32 %293, ptr %291, align 4
  %294 = shl i32 %290, 1
  %295 = and i32 %294, 6
  %indvars.iv.next20.36 = add nuw nsw i64 %indvars.iv19, 37
  %296 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.36
  %297 = load i32, ptr %296, align 4
  %298 = or i32 %297, %295
  %299 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.35
  %300 = lshr i32 110, %298
  %301 = and i32 %300, 1
  store i32 %301, ptr %299, align 4
  %302 = shl i32 %298, 1
  %303 = and i32 %302, 6
  %indvars.iv.next20.37 = add nuw nsw i64 %indvars.iv19, 38
  %304 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.37
  %305 = load i32, ptr %304, align 4
  %306 = or i32 %305, %303
  %307 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.36
  %308 = lshr i32 110, %306
  %309 = and i32 %308, 1
  store i32 %309, ptr %307, align 4
  %310 = shl i32 %306, 1
  %311 = and i32 %310, 6
  %indvars.iv.next20.38 = add nuw nsw i64 %indvars.iv19, 39
  %312 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.38
  %313 = load i32, ptr %312, align 4
  %314 = or i32 %313, %311
  %315 = getelementptr inbounds [80 x i32], ptr %board, i64 0, i64 %indvars.iv.next20.37
  %316 = lshr i32 110, %314
  %317 = and i32 %316, 1
  store i32 %317, ptr %315, align 4
  %exitcond22.not.38 = icmp eq i64 %indvars.iv.next20.38, 79
  br i1 %exitcond22.not.38, label %for.tail.L14, label %for.body.L21

for.tail.L14:                                     ; preds = %for.body.L21
  %318 = add nuw nsw i32 %i1.016, 1
  %exitcond23.not = icmp eq i32 %318, 78
  br i1 %exitcond23.not, label %for.exit.L14, label %for.head.L15.preheader

for.exit.L14:                                     ; preds = %for.tail.L14
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) local_unnamed_addr #1

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #2

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nocallback nofree nounwind willreturn memory(argmem: write) }
