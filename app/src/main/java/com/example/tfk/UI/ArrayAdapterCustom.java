package com.example.tfk.UI;

import android.content.Context;
import android.graphics.Paint;
import android.os.AsyncTask;
import android.text.Layout;
import android.util.Log;
import android.util.TypedValue;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import com.example.tfk.R;

import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class ArrayAdapterCustom extends ArrayAdapter<Cards> {
    Context mContext;

    public TextView[] bodyArray;


    public ArrayAdapterCustom(Context context, int resourceId, List<Cards> items) {
        super(context, resourceId, items);
        bodyArray = new TextView[100];
    }

    public View getView(int position, View convertView, ViewGroup parent) {
        Cards card_item = getItem(position);

        if (convertView == null) {
            convertView = LayoutInflater.from(getContext()).inflate(R.layout.card, parent, false);
        }

        TextView title = (TextView) convertView.findViewById(R.id.titleText);
        TextView body = (TextView) convertView.findViewById(R.id.bodyText);
        TextView link = (TextView) convertView.findViewById(R.id.linkText);

        bodyArray[position] = body;

        title.setText(card_item.getTitle());
//        body.setText(card_item.getBody()[0]);
        splitUpBodyText(body, card_item, card_item.getBody()[0]);
        link.setText(card_item.getLink());


        return convertView;
    }


    private void splitUpBodyText(TextView body, Cards card_item, String bodyText) {
        body.setText(bodyText);
        final String[] bodyTextFinal = {bodyText};

        ViewTreeObserver vto;
        vto = body.getViewTreeObserver();
        vto.addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
          @Override
          public void onGlobalLayout() {
              SplitUpBodyText split = new SplitUpBodyText(body, card_item, bodyText);
              split.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
          }

        });


    }


//        ExecutorService service = Executors.newFixedThreadPool(4);
//        service.submit(new Runnable() {
//        new Thread(new Runnable() {
//            public void run() {

//                body.setText(bodyText);
//                final String[] bodyTextFinal = {bodyText};
//
//                ViewTreeObserver vto = body.getViewTreeObserver();
//                vto.addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
//                    @Override
//                    public void onGlobalLayout() {
//                        Layout layout;
//                        String bodyText = bodyTextFinal[0];
//
//                        while (true) {
//                            body.setText(bodyText);
//                            layout = body.getLayout();
//                            if (layout != null) {
//                                int lines = layout.getLineCount();
//                                if (lines > 0) {
//                                    int ellipsisCount = layout.getEllipsisCount(lines - 1);
//                                    if (ellipsisCount > 0) {
//
//                                        bodyText = bodyText.substring(0, bodyText.length() - 1);
////                                System.out.println("Subtracted text: " + bodyText);
//
//
//                                    } else break;
//
//                                }
//
//                            }
//
//                        }
//
//                    }
//                });

//            }
//        }).start();
//        }
//    }
//
//    public void splitUpBodyText(TextView body, Cards card_item, String bodyText) {
//
//
//        body.setText(bodyText);
//        final String[] bodyTextFinal = {bodyText};
//
//        ViewTreeObserver vto = body.getViewTreeObserver();
//        vto.addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
//            @Override
//            public void onGlobalLayout() {
//                Layout layout = body.getLayout();
//                String bodyText = bodyTextFinal[0];
//
//                while(true){
//                    body.setText(bodyText);
//                    layout = body.getLayout();
//                    if(layout != null) {
//                        int lines = layout.getLineCount();
//                        if(lines > 0) {
//                            int ellipsisCount = layout.getEllipsisCount(lines - 1);
//                            if ( ellipsisCount > 0) {
//
//                                bodyText = bodyText.substring(0, bodyText.length() - 20 );
////                                System.out.println("Subtracted text: " + bodyText);
//
//
//                            }
//
//                            else break;
//
//                        }
//
//                    }
//
//                }
//
//            }
//        });
//
//
//    }
//
//
}
